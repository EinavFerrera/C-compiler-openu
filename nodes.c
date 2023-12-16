#include <stdio.h>
#include <stdlib.h>
#include "genericH.h"
#include "nodes.h"


typedef struct node
{
	char originalLine[MAX_LINE_SIZE];  /*original line from the file*/
	int lineNum;		    /*line number in the original file*/
	int lableAddressLine;   /*address of label in ob*/
	int operandCount;   	/*number of operands if its a command*/
	int operandType[2];     /*[0] - operand 1. [1] - operand 2|| imm,reg,label*/
	int operandImm[2];	    /*if the operand is number*/
    int operandReg[2]; 	    /*if the operand is register*/
    char *operandLabel[2];  /*if the operand is label*/
	int ARE;			    /*A/R/E address*/
	char labelText[MAX_LINE_SIZE];/*original label*/
    int lineSize;	        /*number of words needed in the IC\DC*/
    int lineType;          /*type of the instruction - DATA/CODE/ENTRY/EXT*/
	node next;			    /*pointer to the next node in list*/
} item;

typedef struct isMCR
{
	char mcrName[MAX_LINE_SIZE];
	
	int numOfLines;	
}

node createNewNode(char *line ,int lineAdress)
{
	node newNode = (node)calloc(1, sizeof(item));
    strcpy(newNode->originalLine, line);  
	newNode->lineNum = 	lineAdress;	
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
	newNode->ARE = NULL;			    
	newNode->lineSize = NULL;	      
	newNode->lineType = NULL;        
	newNode->next = NULL;	
	return newNode;
}

void nodeInit(node *node,node *head){


}