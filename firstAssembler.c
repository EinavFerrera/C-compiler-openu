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

    fclose(inputFile);
}

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

// checks if label is valid
void createLabelsList(cNode *head)
{
}
cNode *searchNode(cNode *head, char *target, int srcAttr)
{
    cNode temp = head;
    while (temp != NULL)
    {
        switch (srcAttr)
        {
        case ORIGINAL_LINE:
            // code for ORIGINAL_LINE attribute
            break;
        case LINE_NUM:
            // code for LINE_NUM attribute
            break;
        case LINE_TYPE:
            // code for LINE_TYPE attribute
            break;
        case LABEL_TEXT:
            // code for LABEL_TEXT attribute
            break;
        case OP_CODE:
            // code for OP_CODE attribute
            break;
        case OPERAND_COUNT:
            // code for OPERAND_COUNT attribute
            break;
        case OPERAND_TYPE_1:
            // code for OPERAND_TYPE_1 attribute
            break;
        case OPERAND_TYPE_2:
            // code for OPERAND_TYPE_2 attribute
            break;
        case OPERAND_LABEL_1:
            // code for OPERAND_LABEL_1 attribute
            break;
        case OPERAND_LABEL_2:
            // code for OPERAND_LABEL_2 attribute
            break;
        case OPERAND_IMM_1:
            // code for OPERAND_IMM_1 attribute
            break;
        case OPERAND_IMM_2:
            // code for OPERAND_IMM_2 attribute
            break;
        case OPERAND_REG_1:
            // code for OPERAND_REG_1 attribute
            break;
        case OPERAND_REG_2:
            // code for OPERAND_REG_2 attribute
            break;
        case TARGET_LABEL_ADDRESS:
            // code for TARGET_LABEL_ADDRESS attribute
            break;
        case LABLE_ADDRESS_LINE:
            // code for LABLE_ADDRESS_LINE attribute
            break;
        case ARE:
            // code for ARE attribute
            break;
        case LINE_SIZE:
            // code for LINE_SIZE attribute
            break;
        case NUM_OF_WORDS:
            // code for NUM_OF_WORDS attribute
            break;
        case DATA_ARRAY:
            // code for DATA_ARRAY attribute
            break;
        case ERROR_COUNT:
            // code for ERROR_COUNT attribute
            break;
        }

        if (strcmp(temp->labelText, target) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
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