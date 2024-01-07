#include "genericH.h"

void fAssembler(char *fileName)
{

    // Open the input file
    char fileNameAm[MAX_LINE_SIZE];
    strcpy(fileNameAm, fileName);
    strcat(fileNameAm, ".as"); // TODO: change to .am
    FILE *inputFile = fopen(fileNameAm, "r");
    if (inputFile == NULL)
    {
        printf("Failed to open the file \"%s\".\n", fileName);
        return;
    }
    char line[MAX_LINE_SIZE];
    int lineCounter = 1;
    cNode headNode = NULL;
    cNode newNode;

    while (fgets(line, sizeof(line), inputFile))
    {
        newNode = createNewNode(line, lineCounter, &headNode);
        nodeInit(&newNode, &headNode);
        lineCounter++;
    }
    // checks if label is valid
    // checks if there are no extra commas in the end of line
    // checks if there are no double commas between oprands or lables
    // checks if there are no extra commas between lable to CODE or DATA
    // checks id there is no more than one string in .string
    // no labels in .ent and .ext
    // label in entry is valid and exists
    // no same labler as entry and as extern
    // no duplicated labels DATA
    // no spaces between label and :
    // checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01
    // check if the lables used are exists as ENTRY, EXT, or DATA
    // check if the data lables has engouth vars in the array as we try to use
    // check the count of par, thier types and locations are valid for the CODE, add numOFLines to the node
    // data lable = 2 words in memory ( one for the label and one for the imm)
    // lablesChain(&headNode);

    printLoop(headNode);
    printf("************ Pre Assembler finished succssesfully for file %s! ************\n", fileName);
    lNode labelListDeclared = NULL;
    lNode newListNode;
    int srcList[] = {LABEL_TEXT, OPERAND_TYPE_1, OPERAND_TYPE_2, LINE_TYPE};
    labelListDeclared = createLabelsList(headNode, labelListDeclared, srcList, 4);
    printListLoop(labelListDeclared);
    printf("************ Searching your Attr\n");
    fclose(inputFile);
}
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr)
{
    cNode newTempNode = (cNode)calloc(1, sizeof(contentNode_object));
    for (int i = 0; i < numOfAttr; i++)
    {
        cNode temp = orgListHead;
        printf("\t\t\t\t\t\t\tirritation number %d looking for: %d\n", i, listAttr[i]);
        while (temp != NULL)
        {
            lNode newListNode = (lNode)calloc(1, sizeof(list_object));
            newListNode->name[0] = '\0';
            newTempNode = searchNode(temp, "", listAttr[i]);

            if (newTempNode != NULL)
            {
                printNode(newTempNode);
                if (listAttr[i] == LABEL_TEXT && (newTempNode->labelText[0] != '\0'))
                {
                    printf("1\n");
                    strcpy(newListNode->name, newTempNode->labelText);
                    newListNode->declarationError = 1;
                    newListNode->type = DECLERATION;
                }
                else if (
                    (listAttr[i] == OPERAND_TYPE_1) && ((newTempNode->operandType[0] == LABEL) || (newTempNode->operandType[0] == DATA_LABEL)))
                {
                    printf("2\n");
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = -1;
                    newListNode->type = USED_LABEL;
                }
                else if (
                    (listAttr[i] == OPERAND_TYPE_2) && ((newTempNode->operandType[1] == LABEL) || (newTempNode->operandType[1] == DATA_LABEL)))
                {
                    printf("3\n");
                    strcpy(newListNode->name, newTempNode->operandLabel[1]);
                    newListNode->declarationError = -1;
                    newListNode->type = USED_LABEL;
                }
                else if ((listAttr[i] == LINE_TYPE) && (newTempNode->lineType == ENTRY))
                {
                    printf("4\n");
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = -1;
                    newListNode->type = USED_LABEL;
                }
                else if ((listAttr[i] == LINE_TYPE) && (newTempNode->lineType == EXT))
                {
                    printf("5\n");
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = 1;
                    newListNode->type = DECLERATION;
                }
                if (newListNode->name[0] != '\0')
                {
                    printf("6\n");
                    newListNode->next = newListHead;
                    newListHead = newListNode;
                    newListNode->lineNumber = newTempNode->lineNum;
                }
            }
            temp = newTempNode->next;
        }
    }
    return newListHead;
}
// int srcAtrr;
// int i;
// char target[MAX_LABEL_SIZE];
// printf("Enter the attribute num you want to search:\n");
// scanf("%d", &srcAtrr);
// printf("Enter the target:\n");
// scanf("%s", target);
// printf("Enter the num of iterations:\n");
// scanf("%d", &i);
// cNode foundNode = headNode;
// for (i; i > 0; i--)
// {
//     foundNode = searchNode(foundNode, target, srcAtrr);
//     if (foundNode == NULL)
//         printf("Node with attribute '' equals to ' not found.\n");
//     else
//     {
//         printNode(foundNode);
//         foundNode = foundNode->next;
//     }
// }

// checks if label is valid

cNode *searchNode(cNode head, char *target, int srcAttr)
{
    cNode *temp = &head;
    while ((*temp) != NULL)
    {
        switch (srcAttr)
        {
        case ORIGINAL_LINE:
            if (strcmp(target, "") == 0)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && strcmp((*temp)->originalLine, target) == 0)
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case LINE_NUM:
            if (strcmp(target, "") == 0)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->lineNum == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case LINE_TYPE:
            if (strcmp(target, "") == 0 && (*temp)->lineType != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->lineType == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case LABEL_TEXT:
            if (strcmp(target, "") == 0 && ((*temp)->labelText)[0] != '\0')
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (strcmp((*temp)->labelText, target) == 0))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;

        case OP_CODE:
            if (strcmp(target, "") == 0 && (*temp)->opCode != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->opCode == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_COUNT:
            if (strcmp(target, "") == 0 && (*temp)->operandCount != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandCount == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_TYPE_1:
            if (strcmp(target, "") == 0 && (*temp)->operandType[0] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandType[0] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_TYPE_2:
            if (strcmp(target, "") == 0 && (*temp)->operandType[1] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandType[1] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_LABEL_1:
            if (strcmp(target, "") == 0 && (*temp)->operandLabel[0][0] != '\0')
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && strcmp((*temp)->operandLabel[0], target) == 0)
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_LABEL_2:
            if (strcmp(target, "") == 0 && (*temp)->operandLabel[1][0] != '\0')
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && strcmp((*temp)->operandLabel[1], target) == 0)
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_IMM_1:
            if (strcmp(target, "") == 0 && (*temp)->operandImm[0] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandImm[0] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_IMM_2:
            if (strcmp(target, "") == 0 && (*temp)->operandImm[1] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandImm[1] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_REG_1:
            if (strcmp(target, "") == 0 && (*temp)->operandReg[0] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandReg[0] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case OPERAND_REG_2:
            if (strcmp(target, "") == 0 && (*temp)->operandReg[1] != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->operandReg[1] == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case TARGET_LABEL_ADDRESS:
            if (strcmp(target, "") == 0 && (*temp)->targetLabelAdd != NA)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->targetLabelAdd == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case LABLE_ADDRESS_LINE:
            if ((*temp)->lableAddressLine == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case ARE:
            if ((*temp)->ARE == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case LINE_SIZE:
            if ((*temp)->lineSize == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case NUM_OF_WORDS:
            if ((*temp)->numOfWords == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        case DATA_ARRAY:
            printf("ERROR: searchNode: DATA_ARRAY is not a valid attribute for search\n");
            return NULL;
            temp = &((*temp)->next);
            continue;
        case ERROR_COUNT:
            if (strcmp(target, "") == 0 && (*temp)->errorCount != 0)
            {
                return (*temp);
            }
            else if (strcmp(target, "") != 0 && (*temp)->errorCount == atoi(target))
            {
                return (*temp);
            }
            temp = &((*temp)->next);
            continue;
        default:
            printf("ERROR: searchNode: '%d' is not a valid attribute for search\n", srcAttr);
            return NULL;
            continue;
        }
    }
    return NULL;
}

// checks if there are no extra commas in the end of line
// checks if there are no double commas between oprands or lables
// checks if there are no extra commas between lable to CODE or DATA
// checks id there is no more than one string in .string
// no labels in .ent and .ext
// label in entry is valid and exists
// no same labler as entry and as extern
// no duplicated labels DATA
// no spaces between label and :
// checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01
// check if the lables used are exists as ENTRY, EXT, or DATA
// check if the data lables has engouth vars in the array as we try to use
// check the count of par, thier types and locations are valid for the CODE, add numOFLines to the node
// data lable = 2 words in memory ( one for the label and one for the imm)
// lablesChain(&headNode);

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <ctype.h>
// #include "structures.h"
// #include "registerTable.h"
// #include "firstAssembler.h"
// #include "secondAssembler.h"
// #include "errors.h"

// void fPass(char *fileName)
// {}

// void emptyIntArray(int array[], int n) /* set array to empty */
// {}

// void nextExpression(char **ptr, int *count, int *wordLength, char **word)
// {}

// void ignoreSpaceTab(char **ptr)
// {}

// bool validRow(char *ptr, int lineNum)
// {}

// bool labelCheck(char *ptr, char *word, int count, int wordLength, gNode *hSymbol, gNode temp, int lineNum)
// {}

// bool validLabel(gNode *list, char *ptr, int lineNum)
// {}

// bool isEntryExtern(char *ptr)
// {}

// bool isData(char *ptr)
// {}

// bool isString(char *ptr)
// {}

// int isCommand(char *ptr)
// {}

// int analyzeData(char *ptr, gNode *hRow, int lineNum)
// {}

// int analyzeString(char *ptr, gNode *hRow, int lineNum)
// {}

// int analyzeOperands(bool special, char *ptr, int commandIndex, int *operandType, gNode *hSuspectLabel, gNode *hRow, int lineNum, int IC)
// {}

// int immidiateCheck(char *word, gNode row, int opNum, int lineNum)
// {}

// bool isRegister(char *name, gNode row, int index)
// {}

// bool opernadsTypeCheck(gNode row)
// {}

// bool checkValidInstrucion(char *ptr, int opNum, int commandIndex, int *operandType, int lineNum)
// {}

// bool isSigns(char *ptr)
// {}

// int numCheck(char *ptr, gNode row, int opNum)
// {}

// int wordLen(char *ptr)
// {}

// int endOfLine(char *ptr)
// {}

// void addICtoDataAddress(gNode *HEAD, int IC)
// {}
