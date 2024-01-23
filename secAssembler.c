#include "genericH.h"

int sAssembler(char *fileName, cNode headNode, lNode labelList, int codeLength, int IC, int DC)
{
    /*var decleration section*/

    char fileNameOb[MAX_LINE_SIZE];
    FILE *outputFile;
    char *binaryCode = (char *)calloc(15 * codeLength, sizeof(char)); /*allocating the length of the lines for the binary code*/

    /*------------------------*/

    strcpy(fileNameOb, fileName);
    strcat(fileNameOb, ".ob");
    outputFile = fopen(fileNameOb, "w");
    if (outputFile == NULL)
    {
        printf("Failed to open the file \"%s\".\n", fileName);
        return TRUE;
    }
    if (binaryCode == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return FALSE; /* Return an error code*/
    }

    nodesToBinary(headNode, labelList, binaryCode);
    binaryToSigns(outputFile, binaryCode, IC, DC);
    if (searchListNode(labelList, "20", TYPE) != NULL)
    {
        printf("\t\t\t\t\t\t\t\t\t************ entry label created\n");
        createEntryExternFile(labelList, ENTRY, fileName);
    }
    if (searchListNode(labelList, "21", TYPE) != NULL)
    {
        createEntryExternFile(labelList, EXT, fileName);
        printf("\t\t\t\t\t\t\t\t\t************ extern Label created\n");
    }
    free(binaryCode);
    fclose(outputFile);
    return TRUE;
}
int nodesToBinary(cNode headNode, lNode labelList, char *binaryCode)
{
    char addressAsStr[sizeof(char) * 5] = {"100"};
    char tempNumToStr[15] = {'\0'};
    cNode temp = searchNode(headNode, addressAsStr, LABLE_ADDRESS_LINE);
    int i = 0;
    int p = 0;
    int j = 0;

    while (temp != NULL)
    {
        if (temp->lineType == CODE)
        {
            i = 0;
            for (i = 0; i < temp->lineSize; i++)
            {
                memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                if (i == 0)
                {
                    strcat(binaryCode, "0000");
                    addStrToBinaryCode(tempNumToStr, temp->opCode, 4);
                    strcat(binaryCode, tempNumToStr);
                    memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                    for (p = 0; p < 2; p++)
                    {
                        if (temp->operandCount == 1)
                        {
                            strcat(binaryCode, "00");
                        }
                        switch (temp->operandType[p])
                        {
                        case IMM:
                            strcat(binaryCode, "00");
                            break;
                        case LABEL:
                            strcat(binaryCode, "01");
                            break;
                        case DATA_LABEL:
                            strcat(binaryCode, "10");
                            break;
                        case REG:
                            strcat(binaryCode, "11");
                            break;
                        default:
                            strcat(binaryCode, "00");
                            break;
                        }
                        if (temp->operandCount == 1)
                        {
                            p = 2;
                        }
                    }
                    strcat(binaryCode, "00\n");
                }
                if (i == 1 || i == 2)
                {
                    if (temp->operandType[i - 1] == IMM)
                    {
                        addStrToBinaryCode(tempNumToStr, temp->operandImm[i - 1], 12);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        strcat(binaryCode, "00\n");
                    }
                    else if (temp->operandType[0] == REG && temp->operandType[1] == REG)
                    {
                        strcat(binaryCode, "000000");
                        addStrToBinaryCode(tempNumToStr, temp->operandReg[0], 3);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        addStrToBinaryCode(tempNumToStr, temp->operandReg[1], 3);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        strcat(binaryCode, "00\n");
                    }
                    else if (temp->operandType[i - 1] == REG)
                    {
                        if (i == 1)
                        {
                            strcat(binaryCode, "000000");
                            addStrToBinaryCode(tempNumToStr, temp->operandReg[0], 3);
                            strcat(binaryCode, tempNumToStr);
                            memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                            strcat(binaryCode, "00000\n");
                        }
                        if (i == 2)
                        {
                            strcat(binaryCode, "000000000");
                            addStrToBinaryCode(tempNumToStr, temp->operandReg[0], 3);
                            strcat(binaryCode, tempNumToStr);
                            memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                            strcat(binaryCode, "00\n");
                        }
                    }
                    else if (temp->operandType[i - 1] == LABEL)
                    {
                        addStrToBinaryCode(tempNumToStr, temp->targetLabelAdd[i - 1], 12);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        if (temp->targetLabelAdd[i - 1] == 0)
                        {
                            strcat(binaryCode, "01\n");
                        }
                        else
                        {
                            strcat(binaryCode, "10\n");
                        }
                    }
                    else if (temp->operandType[i - 1] == DATA_LABEL)
                    {
                        addStrToBinaryCode(tempNumToStr, temp->targetLabelAdd[i - 1], 12);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        if (temp->targetLabelAdd[i - 1] == 0)
                        {
                            strcat(binaryCode, "01\n");
                        }
                        else
                        {
                            strcat(binaryCode, "10\n");
                        }
                        addStrToBinaryCode(tempNumToStr, temp->operandImm[i - 1], 12);
                        strcat(binaryCode, tempNumToStr);
                        memset(tempNumToStr, '\0', sizeof(tempNumToStr));
                        strcat(binaryCode, "00\n");
                    }
                }
            }
        }
        if (temp->lineType == DATA_STRING)
        {
            j = 0;
            while (j < temp->numOfWords)
            {
                addStrToBinaryCode(tempNumToStr, temp->dataArray[j], 14);
                strcat(binaryCode, tempNumToStr);
                strcat(binaryCode, "\n");
                j++;
            }
        }
        if (temp->lineType == DATA_INT)
        {
            j = 0;
            while (j < temp->numOfWords)
            {
                addStrToBinaryCode(tempNumToStr, temp->dataArray[j], 14);
                strcat(binaryCode, tempNumToStr);
                strcat(binaryCode, "\n");
                j++;
            }
        }
        sprintf(addressAsStr, "%d", (atoi(addressAsStr) + temp->lineSize));
        temp = searchNode(headNode, addressAsStr, LABLE_ADDRESS_LINE);
    }
    strcat(binaryCode, "\0");
    return TRUE;
}
void addStrToBinaryCode(char *tempStr, int value, int size)
{
    int i = 0;
    while (i <= size)
    {
        if ((value >> i) & 1)
        {
            tempStr[(size - 1) - i] = '1';
        }
        else
        {
            tempStr[(size - 1) - i] = '0';
        }
        i++;
    }
}
void binaryToSigns(FILE *outputFile, char *binaryCode, int IC, int DC)
{
    int i = 1;
    int lineCounter = 0;
    char ICDCStr[5] = {'\0'};
    char tempStr[20] = {'\0'};

    strcat(tempStr, "  ");
    sprintf(ICDCStr, "%d", IC - 100);
    strcat(tempStr, ICDCStr);
    memset(ICDCStr, '\0', sizeof(ICDCStr));
    strcat(tempStr, " ");
    sprintf(ICDCStr, "%d", DC);
    strcat(tempStr, ICDCStr);
    strcat(tempStr, "\n0100  ");
    fputs(tempStr, outputFile);
    while (binaryCode[i] != '\0')
    {
        if (binaryCode[i - 1] == '\n')
        {
            lineCounter++;
            sprintf(tempStr, "\n0%d  ", (100 + lineCounter));
            fputs(tempStr, outputFile);
            memset(ICDCStr, '\0', sizeof(ICDCStr));
            i++;
        }
        else if (binaryCode[i - 1] == '0' && binaryCode[i] == '0')
        {
            putc('*', outputFile);
            i += 2;
        }
        else if (binaryCode[i - 1] == '0' && binaryCode[i] == '1')
        {
            putc('#', outputFile);
            i += 2;
        }
        else if (binaryCode[i - 1] == '1' && binaryCode[i] == '0')
        {
            putc('%', outputFile);
            i += 2;
        }
        else if (binaryCode[i - 1] == '1' && binaryCode[i] == '1')
        {
            putc('!', outputFile);
            i += 2;
        }
    }
}
void createEntryExternFile(lNode labelList, int searchAttr, char *fileName)
{
    FILE *outputFile;
    char fileNameEnt[MAX_LINE_SIZE];
    lNode temp;

    strcpy(fileNameEnt, fileName);
    if (searchAttr == ENTRY)
    {
        strcat(fileNameEnt, ".ent");
    }
    else if (searchAttr == EXT)
    {
        strcat(fileNameEnt, ".ext");
    }
    else
    {
        printf("Error: invalid searchAttr value.\n");
        return;
    }
    outputFile = fopen(fileNameEnt, "w");
    if (outputFile == NULL)
    {
        printf("Failed to open the file \"%s\".\n", fileName);
        return;
    }
    temp = labelList;
    while (temp != NULL)
    {
        if (searchAttr == EXT &&
            temp->mainType == USED_LABEL &&
            temp->address == 0 &&
            temp->EXTUsedAddress != 0)
        {
            fprintf(outputFile, "%s\t%d\n", temp->name, temp->EXTUsedAddress);
        }
        else if (searchAttr == ENTRY &&
                 temp->type == searchAttr)
        {
            fprintf(outputFile, "%s\t%d\n", temp->name, temp->address);
        }
        temp = temp->next;
    }
    fclose(outputFile);
}
