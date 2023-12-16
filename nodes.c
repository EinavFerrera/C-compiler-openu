#include "genericH.h"
#include "nodes.h"

typedef struct contentNode
{
	char originalLine[MAX_LINE_SIZE]; /*original line from the file*/
	int lineNum;					  /*line number in the original file*/
	int lableAddressLine;			  /*address of label in ob*/
	int operandCount;				  /*number of operands if its a command*/
	int operandType[2];				  /*[0] - operand 1. [1] - operand 2|| imm,reg,label*/
	int operandImm[2];				  /*if the operand is number*/
	int operandReg[2];				  /*if the operand is register*/
	char *operandLabel[2];			  /*if the operand is label*/
	int ARE;						  /*A/R/E address*/
	char labelText[MAX_LINE_SIZE];	  /*original label*/
	int lineSize;					  /*number of words needed in the IC\DC*/
	int lineType;					  /*type of the instruction - DATA/CODE/ENTRY/EXT*/
	int opCode;						  /*the codenum of the operation*/
	int numOfWords;					  /*number of words in data line*/
	contentNode next;				  /*pointer to the next node in list*/
} item;

contentNode createNewNode(char *line, int lineAdress)
{
	contentNode newNode = (contentNode)calloc(1, sizeof(item));
	strcpy(newNode->originalLine, line);
	newNode->lineNum = lineAdress;
	newNode->lableAddressLine = NULL;
	newNode->operandCount = NULL;
	newNode->operandType[0] = NULL;
	newNode->operandType[1] = NULL;
	newNode->operandImm[0] = NULL;
	newNode->operandImm[1] = NULL;
	newNode->operandReg[0] = NULL;
	newNode->operandReg[1] = NULL;
	newNode->operandLabel[0] = NULL;
	newNode->operandLabel[1] = NULL;
	newNode->numOfWords = NULL;
	newNode->ARE = NULL;
	newNode->lineSize = NULL;
	newNode->lineType = NULL;
	newNode->opCode = NULL;
	newNode->next = NULL;
	return newNode;
}

void nodeInit(contentNode *node, contentNode *head)
{
	char line[MAX_LINE_SIZE];
	strcpy(line, (*node)->originalLine);
	char *delimiters = " \t,\n";
	char *token = strtok(line, delimiters);
	while (token != NULL)
	{
		typeProperties(*node, token); // sets the lineType = DATA/EXT/ENT
		codeProperties(*node, token); // sets the lineType = DATA/EXT/ENT

		if (token[strlen(token) - 1] == ':')
		{
			if ((*node)->labelText != NULL)
			{
				printf("Error: In line %d - label already exists\n", (*node)->lineNum);
				return;
			}
			strncpy((*node)->labelText, token, strlen(token) - 1);
		}
		else if (token[0] == '#')
		{
			(*node)->operandType[(*node)->operandCount] = IMM;
			(*node)->operandImm[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		else if (token[0] == 'r' && token[1] >= '0' && token[1] <= '7' && strlen(token) == 2)
		{
			(*node)->operandType[(*node)->operandCount] = REG;
			(*node)->operandReg[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
	}
	token = strtok(NULL, delimiters);

	contentNode current = *head;
	if (current == NULL)
	{
		*head = *node;
	}
	else
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = *node;
	}
}

void typeProperties(contentNode *node, char *token)
{
	if (strcmp(token, ".data") == 0)
	{
		(*node)->lineType = DATA;
	}
	else if (strcmp(token, ".string") == 0)
	{
		(*node)->lineType = DATA;
	}
	else if (strcmp(token, ".entry") == 0)
	{
		(*node)->lineType = ENTRY;
	}
	else if (strcmp(token, ".extern") == 0)
	{
		(*node)->lineType = EXT;
	}
	else
	{
		printf("Error: In line %d - unknown directive\n", (*node)->lineNum);
	}
	return;
}
void codeProperties(contentNode *node, char *token)
{
	(*node)->lineType = CODE;
	if (strcmp(token, "mov") == 0)
	{
		(*node)->opCode = 0;
		(*node)->lineSize = 2;
	}
	else if (strcmp(token, "cmp") == 0)
	{
		(*node)->opCode = 1;
		(*node)->lineSize = 2;
	}
	else if (strcmp(token, "add") == 0)
	{
		(*node)->opCode = 2;
		(*node)->lineSize = 2;
	}
	else if (strcmp(token, "sub") == 0)
	{
		(*node)->opCode = 3;
		(*node)->lineSize = 2;
	}
	else if (strcmp(token, "not") == 0)
	{
		(*node)->opCode = 4;
		(*node)->lineSize = 2;
	}
	else if (strcmp(token, "clr") == 0)
	{
		(*node)->opCode = 5;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "lea") == 0)
	{
		(*node)->opCode = 6;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "inc") == 0)
	{
		(*node)->opCode = 7;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "dec") == 0)
	{
		(*node)->opCode = 8;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "jmp") == 0)
	{
		(*node)->opCode = 9;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "bne") == 0)
	{
		(*node)->opCode = 10;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "red") == 0)
	{
		(*node)->opCode = 11;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "prn") == 0)
	{
		(*node)->opCode = 12;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "jsr") == 0)
	{
		(*node)->opCode = 13;
		(*node)->lineSize = 1;
	}
	else if (strcmp(token, "rts") == 0)
	{
		(*node)->opCode = 14;
		(*node)->lineSize = 0;
	}
	else if (strcmp(token, "hlt") == 0)
	{
		(*node)->opCode = 15;
		(*node)->lineSize = 0;
	}
}