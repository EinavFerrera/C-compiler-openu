#include "genericH.h"

cNode createNewNode(char *line, int lineaddress, cNode *head)
{
	cNode newNode = (cNode)malloc(sizeof(contentNode_object));
	char noBlankLine[MAX_LINE_SIZE];
	if (line != NULL)
	{
		clearDuplicateBlankChars(line, noBlankLine);
		strcpy(newNode->originalLine, noBlankLine);
	}
	newNode->lineNum = lineaddress;
	newNode->lableAddressLine = NA;
	newNode->lineType = NA;
	newNode->opCode = NA;
	newNode->operandCount = 0;
	memset(newNode->labelText, '\0', MAX_LABEL_SIZE);
	newNode->operandType[0] = NA;
	newNode->operandType[1] = NA;
	newNode->operandImm[0] = NA;
	newNode->operandImm[1] = NA;
	newNode->operandReg[0] = NA;
	newNode->operandReg[1] = NA;
	memset(newNode->operandLabel[0], '\0', MAX_LABEL_SIZE);
	memset(newNode->operandLabel[1], '\0', MAX_LABEL_SIZE);
	newNode->targetLabelAdd[0] = NA;
	newNode->targetLabelAdd[1] = NA;
	newNode->ARE = NA;
	newNode->errorCount = 0;
	newNode->numOfWords = NA;
	newNode->lineSize = NA;
	newNode->dataArray = NULL;
	newNode->commaErrorFlag = 0;
	newNode->next = *head;
	*head = newNode;
	return newNode;
}
void nodeInit(cNode *node, cNode *head)
{
	/* var decleration section */
	char line[MAX_LINE_SIZE];
	char *token;
	char *delimiters = " \t,\n\r";
	char label[MAX_LABEL_SIZE];
	char *bracketIndex;
	int num = NA;
	int index;

	/*------------------------*/

	strcpy(line, (*node)->originalLine);
	token = strtok(line, delimiters);
	while (token != NULL)
	{
		if (hasSpacesInWord(token))
		{
			printf("%d _%s_\n}", (*node)->lineNum, token);
			printf("Error: In line %d - \"%s\" is not a valid label, a comma may have been missing\n", (*node)->lineNum, token);
			(*node)->errorCount++;
			return;
		}
		clearAllBlanks(token);

		if ((*node)->operandCount > 2)
		{
			printf("Error: In line %d - too many operands in the line\n", (*node)->lineNum);
			(*node)->errorCount++;
			return;
		}

		/* if its a starting label of the line (XYZ:) */
		if (token[strlen(token) - 1] == ':')
		{
			if ((*node)->labelText[0] != '\0')
			{
				printf("Error: In line %d - too many labels declaritions in the line\n", (*node)->lineNum);
				(*node)->errorCount++;
				return;
			}
			strncpy((*node)->labelText, token, strlen(token) - 1);
		}
		/* if its a code line sets the operandCount,optype,opImm,opReg,opLabel,numOfWords,opCode */
		else if ((*node)->opCode == NA && codeProperties(&(*node), token))
		{
			(*node)->lineType = CODE;
		}
		/* if its a immidiate (-4) */
		else if (token[0] == '#' && (*node)->lineType == CODE)
		{
			if (atoi(token + 1) > 2047 || atoi(token + 1) < -2048)
			{
				printf("Error: In line %d - numbers should be between -2048 and 2047. \"%s\" is not a valid number\n", (*node)->lineNum, token);
				(*node)->errorCount++;
			}
			(*node)->operandType[(*node)->operandCount] = IMM;
			(*node)->operandImm[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a register (r4) */
		else if (token[0] == 'r' &&
				 token[1] >= '0' &&
				 token[1] <= '7' &&
				 strlen(token) == 2 &&
				 (*node)->lineType == CODE)
		{
			(*node)->operandType[(*node)->operandCount] = REG;
			(*node)->operandReg[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a data label (XYZ[3]) */
		else if (token[strlen(token) - 1] == ']' &&
				 strchr(token, '[') != NULL &&
				 (*node)->lineType == CODE)
		{
			bracketIndex = strchr(token, '[');
			index = (int)(bracketIndex - token);
			strncpy(label, token, index);
			label[index] = '\0';
			num = atoi(token + index + 1);

			if (label[0] != '\0' && num != NA)
			{
				(*node)->operandType[(*node)->operandCount] = DATA_LABEL;
				strcpy((*node)->operandLabel[(*node)->operandCount], label);
				(*node)->operandImm[(*node)->operandCount] = num;
				(*node)->operandCount++;
			}
		}
		/* if its a opLabel (XYZ) */
		else if (strlen(token) <= MAX_LABEL_SIZE &&
				 isalpha(token[0]) &&
				 (*node)->lineType == CODE)
		{
			if ((*node)->opCode == NA)
			{
				printf("Error: In line %d - unknown commad\n", (*node)->lineNum);
				(*node)->errorCount++;
				return;
			}
			(*node)->operandType[(*node)->operandCount] = LABEL;
			strcpy((*node)->operandLabel[(*node)->operandCount], token);
			(*node)->operandCount++;
		}
		/*if its data/ent/ext label sets the lineType = DATA/EXT/ENT*/
		else if (token[0] == '.')
		{
			typeProperties(&(*node), token);
			if ((*node)->lineType == DATA_INT)
			{
				insertDataIntNode(&(*node), strtok(NULL, "\n"));
				return;
			}
			else if ((*node)->lineType == DATA_STRING)
			{
				insertDataStringNode(&(*node), strtok(NULL, "\""));
				if (strtok(NULL, "\n") != NULL)
				{
					printf("Error: In line %d - too many arguments in .string line\n", (*node)->lineNum);
					(*node)->errorCount++;
					return;
				}
				return;
			}
			else if ((*node)->lineType == ENTRY)
			{
				strcpy(label, strtok(NULL, " \t\n"));
				if (commasCounter((*node)->originalLine) > 0)
				{
					printf("Error: In line %d - no commas allowed in entry line\n", (*node)->lineNum);
					(*node)->errorCount++;
					return;
				}
				if (strtok(NULL, "\n") != NULL)
				{
					printf("Error: In line %d - entry line gets only one lable at the time\n", (*node)->lineNum);
					(*node)->errorCount++;
					return;
				}
				strcpy((*node)->operandLabel[0], label);
				(*node)->operandType[0] = ENTRY;
				(*node)->operandCount++;
				return;
			}
			else if ((*node)->lineType == EXT)
			{
				strcpy(label, strtok(NULL, " \t\n"));
				if (commasCounter((*node)->originalLine) > 0)
				{
					printf("Error: In line %d - no commas allowed in extern line\n", (*node)->lineNum);
					(*node)->errorCount++;
					return;
				}
				if (strtok(NULL, "\n") != NULL)
				{
					printf("Error: In line %d - extern line gets only one lable at the time\n", (*node)->lineNum);
					(*node)->errorCount++;
					return;
				}
				strcpy((*node)->operandLabel[0], label);
				(*node)->operandType[0] = EXT;
				(*node)->operandCount++;
				return;
			}
		}
		/*if its a data line*/
		else
		{
			if (!strcmp(token, "\0"))
			{
				printf("Error: In line %d - blank charachters are no an valid label command\n", (*node)->lineNum);
				(*node)->errorCount++;
				return;
			}
			printf("Error: In line %d - \"%s\" is not a valid label or command\n", (*node)->lineNum, token);
			(*node)->errorCount++;
			return;
		}

		/* if past code and linetype is code*/
		if ((*node)->lineType == CODE)
		{
			token = strtok(NULL, ",\n");
		}
		else
		{
			token = strtok(NULL, delimiters);
		}
	}
	if ((*node)->operandCount != 0 && (*node)->operandCount <= commasCounter((*node)->originalLine))
	{
		printf("Error: In line %d - too many commas.\n", (*node)->lineNum);
		(*node)->errorCount++;
	}
}
void typeProperties(cNode *node, char *token)
{
	if (strcmp(token, ".data") == 0)
	{
		(*node)->lineType = DATA_INT;
	}
	else if (strcmp(token, ".string") == 0)
	{
		(*node)->lineType = DATA_STRING;
	}
	else if (strcmp(token, ".entry") == 0)
	{
		(*node)->lineType = ENTRY;
	}
	else if (strcmp(token, ".extern") == 0)
	{
		(*node)->lineType = EXT;
	}
	else if (token[0] == '.')
	{
		printf("Error: In line %d - unknown directive\n", (*node)->lineNum);
		(*node)->errorCount++;
	}
	return;
}
int codeProperties(cNode *node, char *token)
{
	if (strcmp(token, "mov") == 0)
	{
		(*node)->opCode = 0;
		(*node)->lineSize = 2;
		return TRUE;
	}
	else if (strcmp(token, "cmp") == 0)
	{
		(*node)->opCode = 1;
		(*node)->lineSize = 2;
		return TRUE;
	}
	else if (strcmp(token, "add") == 0)
	{
		(*node)->opCode = 2;
		(*node)->lineSize = 2;
		return TRUE;
	}
	else if (strcmp(token, "sub") == 0)
	{
		(*node)->opCode = 3;
		(*node)->lineSize = 2;
		return TRUE;
	}
	else if (strcmp(token, "not") == 0)
	{
		(*node)->opCode = 4;
		(*node)->lineSize = 2;
		return TRUE;
	}
	else if (strcmp(token, "clr") == 0)
	{
		(*node)->opCode = 5;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "lea") == 0)
	{
		(*node)->opCode = 6;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "inc") == 0)
	{
		(*node)->opCode = 7;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "dec") == 0)
	{
		(*node)->opCode = 8;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "jmp") == 0)
	{
		(*node)->opCode = 9;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "bne") == 0)
	{
		(*node)->opCode = 10;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "red") == 0)
	{
		(*node)->opCode = 11;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "prn") == 0)
	{
		(*node)->opCode = 12;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "jsr") == 0)
	{
		(*node)->opCode = 13;
		(*node)->lineSize = 1;
		return TRUE;
	}
	else if (strcmp(token, "rts") == 0)
	{
		(*node)->opCode = 14;
		(*node)->lineSize = 0;
		return TRUE;
	}
	else if (strcmp(token, "hlt") == 0)
	{
		(*node)->opCode = 15;
		(*node)->lineSize = 0;
		return TRUE;
	}
	return FALSE;
}
void printLoop(cNode head)
{
	cNode currentNode = head;
	while (currentNode != NULL)
	{
		printNode(currentNode);
		currentNode = currentNode->next;
	}
}
void printListLoop(lNode head)
{
	while (head != NULL)
	{
		printf("name:_%s_\tline:_%d_\tmain t:_%d_\ttype:_%d_\terror?:_%d_\t address:_%d_\t EXT address:_%d_ \n", head->name, head->lineNumber, head->mainType, head->type, head->declarationError, head->address, head->EXTUsedAddress);
		head = head->next;
	}
}
void printNode(cNode node)
{
	int i;
	if (node == NULL)
	{
		printf("node is null\n");
		return;
	}
	printf("originalLine: %s\n", node->originalLine);
	printf("lineNum: %d\n", node->lineNum);
	printf("lineType: %d\n", node->lineType);
	printf("operandType[0]: %d\n", node->operandType[0]);
	printf("operandType[1]: %d\n", node->operandType[1]);
	printf("labelText: _%s_\n", node->labelText);
	printf("operandLabel[0]: _%s_\n", node->operandLabel[0]);
	printf("operandLabel[1]: _%s_\n", node->operandLabel[1]);
	printf("opCode: %d\n", node->opCode);
	printf("operandImm[0]: %d\n", node->operandImm[0]);
	printf("operandImm[1]: %d\n", node->operandImm[1]);
	printf("operandCount: %d\n", node->operandCount);
	printf("operandReg[0]: %d\n", node->operandReg[0]);
	printf("operandReg[1]: %d\n", node->operandReg[1]);
	printf("lableAddressLine: %d\n", node->lableAddressLine);
	printf("targetLabelAdd[0]: %d\n", node->targetLabelAdd[0]);
	printf("targetLabelAdd[1]: %d\n", node->targetLabelAdd[1]);
	printf("ARE: %d\n", node->ARE);
	printf("lineSize: %d\n", node->lineSize);
	printf("errorCount: %d\n", node->errorCount);
	printf("numOfWords: %d\n", node->numOfWords);
	for (i = 0; i < node->numOfWords; i++)
	{
		if (node->lineType == DATA_STRING)
		{
			printf("Data[%d]: %c\n", i, node->dataArray[i]);
		}
		else if (node->lineType == DATA_INT)
		{
			printf("Data[%d]: %d\n", i, node->dataArray[i]);
		}
	}
	printf("_______________\n");
}
void insertDataIntNode(cNode *node, char *token)
{
	int numOfCommas;
	(*node)->numOfWords = 0;
	token = strtok(token, ",\n");
	numOfCommas = commasCounter((*node)->originalLine);
	(*node)->dataArray = (int *)malloc(0);
	while (token != NULL)
	{
		(*node)->dataArray = (int *)realloc((*node)->dataArray, sizeof(int) * ((*node)->numOfWords + 1));
		if (hasSpacesInWord(token))
		{
			printf("Error: In line %d - numbers shold be seperated with a comma. \"%s\" is not a valid number\n", (*node)->lineNum, token);
			(*node)->errorCount++;
		}
		clearAllBlanks(token);
		if (atoi(token) > 2047 || atoi(token) < -2048) /* the max number can 12 bits represent in 2' complement*/
		{
			printf("Error: In line %d - numbers should be between -2048 and 2047. \"%s\" is not a valid number\n", (*node)->lineNum, token);
			(*node)->errorCount++;
		}

		if (!isOnlyDigit(token))
		{
			printf("Error: In line %d - only numbers are allowed in .data memory allocation. \"%s\" is not a valid number\n", (*node)->lineNum, token);
			(*node)->errorCount++;
		}
		(*node)->dataArray[(*node)->numOfWords] = atoi(token);
		(*node)->numOfWords++;
		token = strtok(NULL, ",\n");
	}
	if ((numOfCommas >= (*node)->numOfWords) && ((*node)->errorCount == 0))
	{
		printf("Error: In line %d - too many commas in the line\n", (*node)->lineNum);
		(*node)->errorCount++;
		(*node)->commaErrorFlag = 1;
	}

	(*node)->lineSize = (*node)->numOfWords;
}
void insertDataStringNode(cNode *node, char *token)
{
	int strLength;
	int i;
	(*node)->numOfWords = 0;
	strLength = strlen(token);
	(*node)->dataArray = (int *)malloc(0);
	if (!isOnlyAlpha(token))
	{
		printf("Error: In line %d - only characters are allowed in .string memory allocation. \"%s\" has unvalid character\n", (*node)->lineNum, token);
		(*node)->errorCount++;
		return;
	}
	for (i = 0; i < strLength; i++)
	{
		(*node)->dataArray = (int *)realloc((*node)->dataArray, sizeof(int) * ((*node)->numOfWords + 1));
		(*node)->dataArray[i] = token[i];
		(*node)->numOfWords++;
	}
	(*node)->dataArray = (int *)realloc((*node)->dataArray, sizeof(int) * ((*node)->numOfWords + 1));
	(*node)->dataArray[i] = '\0';
	(*node)->numOfWords++;

	(*node)->lineSize = (*node)->numOfWords;
}
int isOnlyDigit(char *token)
{
	int i = 0;
	while (token[i] != '\0')
	{
		if (i == 0 && (token[i] == '-' || token[i] == '+'))
		{
			i++;
			continue;
		}
		if (!isdigit(token[i]))
		{
			return FALSE;
		}
		i++;
	}

	return TRUE;
}
int isOnlyAlpha(char *token)
{
	int i = 0;
	while (token[i] != '\0')
	{
		if (!isalpha(token[i]) && token[i] != ' ' && token[i] != '\t')
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;
}
void setNextNode(cNode node, cNode nextNode)
{
	node->next = nextNode;
}
int commasCounter(char *line)
{
	int i = 0;
	int counter = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ',')
		{
			counter++;
		}
		i++;
	}
	return counter;
}
int hasSpacesInWord(char *line)
{
	int i = 0;
	while (line[i] != '\0' && (line[i] == ' ' || line[i] == '\t'))
	{
		i++;
	}
	while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t')
	{
		i++;
	}
	while (line[i] != '\0')
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			return TRUE;
		}
		i++;
	}
	return FALSE;
}
void clearDuplicateBlankChars(char *line, char *newLine)
{
	int i = 0;
	int newi = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\t')
		{
			line[i] = ' ';
		}
		i++;
	}
	i = 0;
	while (line[i] != '\0')
	{
		if (i == 0)
		{
			while (line[i] == ' ')
			{
				i++;
			}
		}
		if (line[i] == ' ')
		{
			newLine[newi] = line[i];
			newi++;
			while (line[i] != '\0' && line[i] == ' ')
			{
				i++;
			}
		}
		else
		{
			newLine[newi] = line[i];
			i++;
			newi++;
		}
	}
	newLine[newi] = '\0';
}
void clearAllBlanks(char *line)
{
	int i = 0;
	int j = 0;
	char newLine[MAX_LINE_SIZE];
	while (line[i] != '\0')
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			newLine[j] = line[i];
			j++;
		}
		i++;
	}
	newLine[j] = '\0';
	strcpy(line, newLine);
}
