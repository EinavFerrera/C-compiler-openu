#include "genericH.h"
/*
mNode createNewMcrNode(char *name, char *content, mNode *head)
{
	mcrNode newMcrNode = (mcrNode)calloc(1, sizeof(mcrNode) + 1);
	strcpy(newMcrNode->name, name);
	newMcrNode->mcrContent = content;
	mcrNode current = *head;
	if (current == NULL)
	{
		*head = newMcrNode;
	}
	else
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newMcrNode;
	}
	return newMcrNode;
}
*/

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
		return true;
	}
	else if (strcmp(token, "cmp") == 0)
	{
		(*node)->opCode = 1;
		(*node)->lineSize = 2;
		return true;
	}
	else if (strcmp(token, "add") == 0)
	{
		(*node)->opCode = 2;
		(*node)->lineSize = 2;
		return true;
	}
	else if (strcmp(token, "sub") == 0)
	{
		(*node)->opCode = 3;
		(*node)->lineSize = 2;
		return true;
	}
	else if (strcmp(token, "not") == 0)
	{
		(*node)->opCode = 4;
		(*node)->lineSize = 2;
		return true;
	}
	else if (strcmp(token, "clr") == 0)
	{
		(*node)->opCode = 5;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "lea") == 0)
	{
		(*node)->opCode = 6;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "inc") == 0)
	{
		(*node)->opCode = 7;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "dec") == 0)
	{
		(*node)->opCode = 8;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "jmp") == 0)
	{
		(*node)->opCode = 9;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "bne") == 0)
	{
		(*node)->opCode = 10;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "red") == 0)
	{
		(*node)->opCode = 11;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "prn") == 0)
	{
		(*node)->opCode = 12;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "jsr") == 0)
	{
		(*node)->opCode = 13;
		(*node)->lineSize = 1;
		return true;
	}
	else if (strcmp(token, "rts") == 0)
	{
		(*node)->opCode = 14;
		(*node)->lineSize = 0;
		return true;
	}
	else if (strcmp(token, "hlt") == 0)
	{
		(*node)->opCode = 15;
		(*node)->lineSize = 0;
		return true;
	}
	return false;
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
		printf("name:_%s_\tline number:_%d_\ttype::_%d_\terror?:_%d_\n", head->name, head->lineNumber, head->type, head->declarationError);
		head = head->next;
	}
}
void printNode(cNode node)
{
	if (node == NULL)
	{
		printf("node is null\n");
		return;
	}
	printf("originalLine: %s\n", node->originalLine);
	// printf("lineNum: %d\n", node->lineNum);
	// printf("lineType: %d\n", node->lineType);
	// printf("operandType[0]: %d\n", node->operandType[0]);
	// printf("operandType[1]: %d\n", node->operandType[1]);
	// printf("labelText: _%s_\n", node->labelText);
	// printf("operandLabel[0]: %s\n", node->operandLabel[0]);
	// printf("operandLabel[1]: %s\n", node->operandLabel[1]);
	// printf("opCode: %d\n", node->opCode);
	// printf("operandImm[0]: %d\n", node->operandImm[0]);
	// printf("operandImm[1]: %d\n", node->operandImm[1]);
	// printf("operandCount: %d\n", node->operandCount);
	// printf("operandReg[0]: %d\n", node->operandReg[0]);
	// printf("operandReg[1]: %d\n", node->operandReg[1]);
	// printf("lableAddressLine: %d\n", node->lableAddressLine);
	// printf("ARE: %d\n", node->ARE);
	// printf("lineSize: %d\n", node->lineSize);
	// printf("errorCount: %d\n", node->errorCount);
	// printf("numOfWords: %d\n", node->numOfWords);
	// for (int i = 0; i < node->numOfWords; i++)
	// {
	// 	if (node->lineType == DATA_STRING)
	// 	{
	// 		printf("Data[%d]: %c\n", i, node->dataArray[i]);
	// 	}
	// 	else
	// 		printf("Data[%d]: %d\n", i, node->dataArray[i]);
	// }
	// printf("_______________\n");
}
cNode createNewNode(char *line, int lineAdress, cNode *head)
{
	cNode newNode = (cNode)calloc(1, sizeof(contentNode_object));
	if (line != NULL)
	{
		strcpy(newNode->originalLine, line);
	}
	newNode->lineNum = lineAdress;
	newNode->lableAddressLine = NA;
	newNode->lineType = NA;
	newNode->opCode = NA;
	newNode->operandCount = 0;
	newNode->labelText[0] = '\0';
	newNode->operandType[0] = NA;
	newNode->operandType[1] = NA;
	newNode->operandImm[0] = NA;
	newNode->operandImm[1] = NA;
	newNode->operandReg[0] = NA;
	newNode->operandReg[1] = NA;
	newNode->operandLabel[0][0] = '\0';
	newNode->operandLabel[1][0] = '\0';
	newNode->targetLabelAdd = NA;
	newNode->ARE = NA;
	newNode->errorCount = 0;
	newNode->numOfWords = NA;
	newNode->lineSize = NA;
	newNode->next = *head;
	*head = newNode;
	return newNode;
}
void nodeInit(cNode *node, cNode *head)
{
	char line[MAX_LINE_SIZE];
	strcpy(line, (*node)->originalLine);
	char *delimiters = " \t,\n";
	printf("line:_%s_\n", line);
	char var[10];
	char *token = strtok(line, delimiters);
	while (token != NULL)
	{
		if ((*node)->operandCount > 1)
		{
			printf("Error: In line %d - too many operands in the line\n", (*node)->lineNum);
			(*node)->errorCount++;
			return;
		}
		printf("token:_%s_\n", token);

		/* if its a starting label of the line (XYZ:)*/
		if (token[strlen(token) - 1] == ':')
		{
			printf("\t\t\ti am a label\n");
			if ((*node)->labelText[0] != '\0')
			{
				printf("Error: In line %d - too many labels declaritions in the line\n", (*node)->lineNum);
				(*node)->errorCount++;
				return;
			}
			strncpy((*node)->labelText, token, strlen(token) - 1);
		}
		// if its a code line
		else if ((*node)->opCode == NA && codeProperties(&(*node), token)) // if its a code line sets the operandCount,optype,opImm,opReg,opLabel,numOfWords,opCode
		{
			printf("\t\t\ti am a CODE DECLERATION\n");

			(*node)->lineType = CODE;
		}
		/* if its a immidiate (-4)*/
		else if (token[0] == '#')
		{
			printf("\t\t\ti am a IMM\n");

			(*node)->operandType[(*node)->operandCount] = IMM;
			(*node)->operandImm[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a register (r4)*/
		else if (token[0] == 'r' && token[1] >= '0' && token[1] <= '7' && strlen(token) == 2)
		{
			printf("\t\t\ti am a REG\n");

			(*node)->operandType[(*node)->operandCount] = REG;
			(*node)->operandReg[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a data label (XYZ[3])*/
		else if (token[strlen(token) - 1] == ']' && strchr(token, '[') != NULL)
		{
			printf("\t\t\ti am a DATA LABEL\n");

			char label[MAX_LABEL_SIZE];
			char *bracketIndex;
			int num;
			int index;

			bracketIndex = strchr(token, '[');
			index = (int)(bracketIndex - token);
			strncpy(label, token, index);
			num = atoi(token + index + 1);

			if (label != NULL && num != NULL)
			{
				(*node)->operandType[(*node)->operandCount] = DATA_LABEL;
				strcpy((*node)->operandLabel[(*node)->operandCount], label);
				(*node)->operandImm[(*node)->operandCount] = num;
				(*node)->operandCount++;
			}
		}
		/* if its a opLabel (XYZ)*/
		else if (strlen(token) <= MAX_LABEL_SIZE && isalpha(token[0]))
		{
			if ((*node)->opCode == NA)
			{
				printf("Error: In line %d - unknown commad\n", (*node)->lineNum);
				(*node)->errorCount++;
				return;
			}
			printf("\t\t\ti am a opLabel\n");
			(*node)->operandType[(*node)->operandCount] = LABEL;
			strcpy((*node)->operandLabel[(*node)->operandCount], token);
			(*node)->operandCount++;
		}
		// if its data/ent/ext label
		else if (token[0] == '.') // sets the lineType = DATA/EXT/ENT
		{
			printf("\t\t\ti am a DATA DECLERATION\n");
			typeProperties(&(*node), token);
			if ((*node)->lineType == DATA_INT)
			{
				printf("\t\t\ti am a DATA_INT\n");
				insertDataIntNode(&(*node), strtok(NULL, "\n"));
				return;
			}
			else if ((*node)->lineType == DATA_STRING)
			{
				printf("\t\t\ti am a DATA_STRING\n");
				insertDataStringNode(&(*node), strtok(NULL, "\""));
				return;
			}
			else if ((*node)->lineType == ENTRY)
			{
				printf("\t\t\ti am a ENTRY line\n");
				char label[MAX_LABEL_SIZE];
				strcpy(label, strtok(NULL, "\n"));
				strcpy((*node)->operandLabel[0], label);
				(*node)->operandType[0] = ENTRY;
				(*node)->operandCount++;
				return;
			}
			else if ((*node)->lineType == EXT)
			{
				printf("\t\t\ti am a EXT line\n");
				char label[MAX_LABEL_SIZE];
				strcpy(label, strtok(NULL, "\n"));
				strcpy((*node)->operandLabel[0], label);
				(*node)->operandType[0] = EXT;
				(*node)->operandCount++;
				return;
			}
		}
		// if its a data line
		else
		{
			printf("Error: In line %d - \"%s\" is not a valid label\n", (*node)->lineNum, token);
			(*node)->errorCount++;
			return;
		}

		token = strtok(NULL, delimiters);
	}
}
void insertDataIntNode(cNode *node, char *token)
{
	printf("\t\t\ti am a DATA_INT handler\n");
	(*node)->numOfWords = 0;
	token = strtok(token, ",\n");
	while (token != NULL)
	{
		(*node)->dataArray = (int *)realloc((*node)->dataArray, sizeof(int) * ((*node)->numOfWords + 1));
		if (!isOnlyDigit(token))
		{
			printf("Error: In line %d - only numbers are allowed in .data memory allocation. \"%s\" is not a valid number\n", (*node)->lineNum, token);
			(*node)->errorCount++;
		}
		(*node)->dataArray[(*node)->numOfWords] = atoi(token);
		(*node)->numOfWords++;
		token = strtok(NULL, ",\n");
	}

	(*node)->lineSize = (*node)->numOfWords;
}
void insertDataStringNode(cNode *node, char *token)
{
	printf("\t\t\ti am a DATA_String handler\n");
	(*node)->numOfWords = 0;
	int strLength = strlen(token);
	if (!isOnlyAlpha(token))
	{
		printf("Error: In line %d - only characters are allowed in .string memory allocation. \"%s\" has unvalid character\n", (*node)->lineNum, token);
		(*node)->errorCount++;
		return;
	}
	for (int i = 0; i < strLength; i++)
	{

		(*node)->dataArray = (int *)realloc((*node)->dataArray, sizeof(int) * ((*node)->numOfWords + 1));
		(*node)->dataArray[i] = token[i];
		(*node)->numOfWords++;
	}

	(*node)->lineSize = (*node)->numOfWords;
}
int isOnlyDigit(char *token)
{
	int i = 0;
	while (token[i] != '\0')
	{
		if (!isdigit(token[i]))
		{
			return false;
		}
		i++;
	}
	return true;
}
int isOnlyAlpha(char *token)
{
	int i = 0;
	while (token[i] != '\0')
	{
		if (!isalpha(token[i]))
		{
			return false;
		}
		i++;
	}
	return true;
}
void setNextNode(cNode node, cNode nextNode)
{
	node->next = nextNode;
}
void addNode(cNode head, cNode newNode)
{
	cNode *current = &head;
	printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tinside Adding node :_%d_%d\n", &head, current);
	if ((*current) == NULL)
	{
		head = newNode;
		printNode(head);
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tadded first Node\n");
	}
	/*
	else
	{
		while (current->next != NULL)
		{
			printf("adding sec or more node \n");
			setNextNode(current, newNode);
		}
		setNextNode(current, newNode);
	}*/
}