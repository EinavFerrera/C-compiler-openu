#include <stdio.h>
#include <stdlib.h>
#include "genericH.h"
#include "nodes.h"

mcrNode createNewMcrNode(char *name, char *content, mcrNode *head)
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

/*
@param line - line content
@param lineAdress - number of the line*/
contentNode createNewNode(char *line, int lineAdress)
{
	contentNode newNode = (contentNode)calloc(1, sizeof(contentNode));
	if (line != NULL)
	{
		strcpy(newNode->originalLine, line);
	}
	newNode->lineNum = lineAdress;
	newNode->lableAddressLine = NA;
	newNode->operandCount = NA;
	newNode->labelText[0] = '\0';
	newNode->operandType[0] = NA;
	newNode->operandType[1] = NA;
	newNode->operandImm[0] = NA;
	newNode->operandImm[1] = NA;
	newNode->operandReg[0] = NA;
	newNode->operandReg[1] = NA;
	newNode->operandLabel[0][0] = '\0';
	newNode->operandLabel[1][0] = '\0';
	newNode->numOfWords = NA;
	newNode->ARE = NA;
	newNode->lineSize = NA;
	newNode->lineType = NA;
	newNode->opCode = NA;
	newNode->next = NULL;
	return newNode;
}

void nodeInit(contentNode *node, contentNode *head)
{
	char line[MAX_LINE_SIZE];
	strcpy(line, (*node)->originalLine);
	char *delimiters = " \t,\n";
	char *token = strtok(line, delimiters);

	/*make the last node in line*/
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

	while (token != NULL)
	{
		typeProperties(*node, token); // sets the lineType = DATA/EXT/ENT
		codeProperties(*node, token); // if its a code line sets the operandCount,optype,opImm,opReg,opLabel,numOfWords,opCode

		/* if its a starting label of the line (XYZ:)*/

		if (token[strlen(token) - 1] == ':' && strlen(token) <= MAX_LABEL_SIZE)
		{
			if ((*node)->labelText != NULL)
			{
				printf("Error: In line %d - label declaration already exists\n", (*node)->lineNum);
				return;
			}
			strncpy((*node)->labelText, token, strlen(token) - 1);
		}
		/* if its a immidiate (-4)*/

		else if (token[0] == '#')
		{
			(*node)->operandType[(*node)->operandCount] = IMM;
			(*node)->operandImm[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a register (r4)*/

		else if (token[0] == 'r' && token[1] >= '0' && token[1] <= '7' && strlen(token) == 2)
		{
			(*node)->operandType[(*node)->operandCount] = REG;
			(*node)->operandReg[(*node)->operandCount] = atoi(token + 1);
			(*node)->operandCount++;
		}
		/* if its a label (XYZ)*/

		else if (strlen(token) <= MAX_LABEL_SIZE && isalpha(token[0]))
		{
			(*node)->operandType[(*node)->operandCount] = LABEL;
			strcpy((*node)->operandLabel[(*node)->operandCount], token);
			(*node)->operandCount++;
		}
		/* if its a data label (XYZ[3])*/
		else if (token[strlen(token) - 1] == ']' && strchr(token, '[') != NULL)
		{
			char *label = strtok(token, "[");
			char *numStr = strtok(NULL, "]");
			if (label != NULL && numStr != NULL)
			{
				(*node)->operandType[(*node)->operandCount] = DATA_LABEL;
				strcpy((*node)->operandLabel[(*node)->operandCount], label);
				(*node)->operandImm[(*node)->operandCount] = atoi(numStr);
				(*node)->operandCount++;
			}
		}
		token = strtok(NULL, delimiters);
	}
}

void typeProperties(contentNode *node, char *token)
{
	if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0)
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
	else if (token[0] == '.')
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
void insertNode(contentNode *head, contentNode *tail, contentNode newNode)
{
	if (*head != NULL)
	{
		(*head)->next = &newNode;
		*tail = &newNode;
	}
	else
	{
		(*tail)->next = newNode;
		*tail = &newNode;
	}
}
void setNext(contentNode list, contentNode toSet)
{
	list->next = getNext(toSet);
}
contentNode *getNext(contentNode list)
{
	return list->next;
}
void printNode(contentNode node)
{
	printf("***************************************************************************\n");
	printf("originalLine: %s\n", node->originalLine);
	// printf("lineNum: %d\n", node->lineNum);
	// printf("lineType: %d\n", node->lineType);
	// printf("labelText: %s\n", node->labelText);
	// printf("operandLabel[0]: %s\n", node->operandLabel[0]);
	// printf("operandLabel[1]: %s\n", node->operandLabel[1]);
	// printf("opCode: %d\n", node->opCode);
	// printf("operandImm[0]: %d\n", node->operandImm[0]);
	// printf("operandImm[1]: %d\n", node->operandImm[1]);
	// printf("operandCount: %d\n", node->operandCount);
	// printf("operandType[0]: %d\n", node->operandType[0]);
	// printf("operandType[1]: %d\n", node->operandType[1]);
	// printf("operandReg[0]: %d\n", node->operandReg[0]);
	// printf("operandReg[1]: %d\n", node->operandReg[1]);
	// printf("lableAddressLine: %d\n", node->lableAddressLine);
	// printf("ARE: %d\n", node->ARE);
	// printf("lineSize: %d\n", node->lineSize);
	// printf("numOfWords: %d\n", node->numOfWords);
	// printf("***************************************************************************\n");
}