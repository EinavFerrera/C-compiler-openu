#include "genericH.h"

void fAssembler(char *fileName)
{
    /* Open the input file*/
    char fileNameAm[MAX_LINE_SIZE];
    strcpy(fileNameAm, fileName);
    strcat(fileNameAm, ".as"); /* TODO: change to .am*/
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
        if (newNode->lineType == CODE)
        {
            validOperandPerCode(newNode);
        }
        lineCounter++;
    }
    printf("\t\t\t\t\t\t\t\t\t************ code to nodes finished for file %s! \n", fileName);
    if (searchNode(headNode, "", ERROR_COUNT) != NULL)
    {
        printf("\t\t\t\t\t\t\t\t\t************ There are errors in the file %s! \n", fileName);
        return;
    }
    /* checks if label are valid - declared and used, no double declerationsm no data label out of limits*/
    int IC = 100;
    int DC = 0;
    numOfLinesAssign(headNode, &IC, &DC);
    fixLineaddress(headNode, IC);
    printf("\t\t\t\t\t\t\t\t\t************ address filled\n");

    lNode labelList = NULL;
    int srcList[] = {LABEL_TEXT, OPERAND_TYPE_1, OPERAND_TYPE_2, LINE_TYPE};
    int srcListLenght = sizeof(srcList) / sizeof(srcList[0]);
    labelList = createLabelsList(headNode, labelList, srcList, srcListLenght);
    if (!validateLabelList(labelList, headNode))
    {
        return;
    }
    printf("\t\t\t\t\t\t\t\t\t************ label list is valid\n");

    /*    checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01*/
    addLabelsAddressToCnode(headNode, labelList);
    printf("\t\t\t\t\t\t\t\t\t************ ARE assigned\n");
    fclose(inputFile);
    sAssembler(fileName, headNode, labelList, IC + DC, IC, DC);
    freeAll(headNode, labelList);
}
void numOfLinesAssign(cNode headNode, int *IC, int *DC)
{
    cNode temp = headNode;
    if (temp != NULL)
    {
        numOfLinesAssign(temp->next, &(*IC), &(*DC));
    }
    else
    {
        return;
    }
    if (temp->lineType == DATA_INT)
    {
        temp->lineSize = temp->numOfWords;
        temp->lableAddressLine = (*DC);
        (*DC) += temp->lineSize;
    }
    else if (temp->lineType == DATA_STRING)
    {
        temp->lineSize = temp->numOfWords;
        temp->lableAddressLine = (*DC);
        (*DC) += temp->lineSize;
    }
    else if (temp->lineType == CODE)
    {
        temp->numOfWords = 1;
        if (temp->operandType[0] == REG &&
            temp->operandType[1] == REG)
        {
            temp->numOfWords++;
        }
        else
        {
            if (temp->operandType[0] == IMM ||
                temp->operandType[0] == REG ||
                temp->operandType[0] == LABEL)
            {
                temp->numOfWords++;
            }
            if (temp->operandType[1] == IMM ||
                temp->operandType[1] == REG ||
                temp->operandType[1] == LABEL)
            {
                temp->numOfWords++;
            }
            if (temp->operandType[0] == DATA_LABEL &&
                temp->operandType[1] == DATA_LABEL)
            {
                temp->numOfWords += 2;
            }
            if (temp->operandType[0] == DATA_LABEL ||
                temp->operandType[1] == DATA_LABEL)
            {
                temp->numOfWords += 2;
            }
        }
        temp->lineSize = temp->numOfWords;
        temp->lableAddressLine = (*IC);
        (*IC) += temp->lineSize;
    }
    else if (temp->lineType == ENTRY ||
             temp->lineType == EXT)
    {
        temp->numOfWords = 0;
    }
}
void fixLineaddress(cNode headNode, int IC)
{
    cNode temp = headNode;
    while (temp != NULL)
    {
        if (temp->lineType == DATA_INT || temp->lineType == DATA_STRING)
        {
            temp->lableAddressLine += IC;
        }
        temp = temp->next;
    }
}
void addaddressToLabelList(lNode labelListhead, cNode headNode)
{
    lNode temp = labelListhead;
    while (temp != NULL)
    {
        if (temp->type == DATA_LABEL)
        {
            temp->address += 3;
        }
        temp = temp->next;
    }
}
void validOperandPerCode(cNode node)
{
    /* 0 = IMM , 1 = LABEL , 2 = DATA LABEL , 3 = REG
    1. src . 2. dst
    1  cmp = 2 operands, 1. IMM, LABEL, DATA LABEL, REG,    2. IMM, LABEL,DATA LABEL, REG
    0  mov = 2 operands, 1. IMM, LABEL, DATA LABEL, REG,    2.      LABEL,DATA LABEL, REG
    2  add = 2 operands, 1. IMM, LABEL, DATA LABEL, REG,    2.      LABEL,DATA LABEL, REG
    3  sub = 2 operands, 1. IMM, LABEL, DATA LABEL, REG,    2.      LABEL,DATA LABEL, REG
    6  lea = 2 operands, 1. LABEL, DATA LABEL               2.      LABEL,DATA LABEL, REG
    12 prn = 1 operand,                                     2. IMM, LABEL,DATA LABEL, REG
    4  not = 1 operand,                                     2.      LABEL,DATA LABEL, REG
    5  clr = 1 operand,                                     2.      LABEL,DATA LABEL, REG
    7  inc = 1 operand,                                     2.      LABEL,DATA LABEL, REG
    8  dec = 1 operand,                                     2.      LABEL,DATA LABEL, REG
    11 red = 1 operand,                                     2.      LABEL,DATA LABEL, REG
    9  jmp = 1 operand,                                     2.      LABEL,            REG
    10 bne = 1 operand,                                     2.      LABEL,            REG
    13 jsr = 1 operand,                                     2.      LABEL,            REG
    14 rts = 0 operand
    15 hlt = 0 operand
    */
    switch (node->opCode)
    {
    case 1: /* cmp*/
        if (node->operandCount != 2)
        {
            printf("ERROR: In line %d - command 'cmp' should have 2 operands.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        if (node->operandType[0] != IMM &&
            node->operandType[0] != LABEL &&
            node->operandType[0] != DATA_LABEL &&
            node->operandType[0] != REG)
        {
            printf("ERROR: In line %d - command 'cmp' first operand should be IMM, LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }

        if (node->operandType[1] != IMM &&
            node->operandType[1] != LABEL &&
            node->operandType[1] != DATA_LABEL &&
            node->operandType[1] != REG)
        {
            printf("ERROR: In line %d - command 'cmp' second operand should be IMM, LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    case 0: /* mov*/
    case 2: // add
    case 3: // sub
        if (node->operandCount != 2)
        {
            printf("ERROR: In line %d - command 'mov/add/sub' should have 2 operands.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        // Check the validity of the first operand
        if (node->operandType[0] != IMM &&
            node->operandType[0] != LABEL &&
            node->operandType[0] != DATA_LABEL &&
            node->operandType[0] != REG)
        {
            printf("ERROR: In line %d - command 'mov/add/sub' first operand should be IMM, LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }

        // Check the validity of the second operand
        if (node->operandType[1] != LABEL &&
            node->operandType[1] != DATA_LABEL &&
            node->operandType[1] != REG)
        {
            printf("ERROR: In line %d - command 'mov/add/sub' second operand should be LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    case 6: // lea
        if (node->operandCount != 2)
        {
            printf("ERROR: In line %d - command 'lea' should have 2 operands.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        // Check the validity of the first operand
        if (node->operandType[0] != LABEL &&
            node->operandType[0] != DATA_LABEL)
        {
            printf("ERROR: In line %d - command 'lea' first operand should be LABEL or DATA LABEL.\n", node->lineNum);
            node->errorCount++;
        }

        // Check the validity of the second operand
        if (node->operandType[1] != LABEL &&
            node->operandType[1] != DATA_LABEL &&
            node->operandType[1] != REG)
        {
            printf("ERROR: In line %d - command 'lea' second operand should be LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;
    case 12: // prn
        if (node->operandCount != 1)
        {
            printf("ERROR: In line %d - command 'prn' should have 1 operand.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        // Check the validity of the operand
        if (node->operandType[0] != IMM &&
            node->operandType[0] != LABEL &&
            node->operandType[0] != DATA_LABEL &&
            node->operandType[0] != REG)
        {
            printf("ERROR: In line %d - command 'prn' operand should be IMM, LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    case 4:  // not
    case 5:  // clr
    case 7:  // inc
    case 8:  // dec
    case 11: // red
        if (node->operandCount != 1)
        {
            printf("ERROR: In line %d - command not/clr/inc/dec/red should have 1 operand.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        // Check the validity of the operand
        if (node->operandType[0] != LABEL &&
            node->operandType[0] != DATA_LABEL &&
            node->operandType[0] != REG)
        {
            printf("ERROR: In line %d - command not/clr/inc/dec/red operand should be LABEL, DATA LABEL, or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    case 9:  // jmp
    case 10: // bne
    case 13: // jsr
        if (node->operandCount != 1)
        {
            printf("ERROR: In line %d - command jmp/bne/jsr should have 1 operand.\n", node->lineNum);
            node->errorCount++;
            return;
        }

        // Check the validity of the operand
        if (node->operandType[0] != LABEL &&
            node->operandType[0] != REG)
        {
            printf("ERROR: In line %d - command jmp/bne/jsr operand should be LABEL or REG.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    case 14: // rts
    case 15: // hlt
        if (node->operandCount != 0)
        {
            printf("ERROR: In line %d - command rts/hlt should have 0 operands.\n", node->lineNum);
            node->errorCount++;
        }
        break;

    default:
        // Handle cases for opcodes without operands
        printf("ERROR: In line %d - opCode '%d' isn't recognize.\n", node->lineNum, node->opCode);
        node->errorCount++;
        break;
    }
}
void AREAssign(cNode headNode, lNode labelList)
{
    // checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01
    cNode temp = headNode;
    while (temp != NULL)
    {
        int p = 0;

        if (temp->lineType != CODE)
        {
            temp = temp->next;
            continue;
        }
        if (((temp->operandType[0] == IMM || temp->operandType[0] == REG) &&
             (temp->operandType[1] == IMM || temp->operandType[1] == REG)) ||
            ((temp->operandCount == 1 && (temp->operandType[0] == REG || temp->operandType[0] == IMM))) ||
            (temp->operandCount == 0))
        {
            temp->ARE = 00;
        }
        else if (temp->operandType[0] == LABEL || temp->operandType[0] == DATA_LABEL)
        {

            lNode tempLabel = searchListNode(labelList, temp->operandLabel[0], LABEL_TEXT);
            while (tempLabel != NULL)
            {

                if (tempLabel->type == EXT)
                {
                    temp->ARE = 01;
                    break;
                }
                else if (tempLabel->mainType == DECLERATION)
                {
                    temp->ARE = 10;
                    break;
                }
                tempLabel = searchListNode(tempLabel->next, temp->operandLabel[0], LABEL_TEXT);
            }
        }
        else if (temp->operandType[1] == LABEL || temp->operandType[1] == DATA_LABEL)
        {
            lNode tempLabel = searchListNode(labelList, temp->operandLabel[1], LABEL_TEXT);
            while (tempLabel != NULL)
            {
                if (tempLabel->type == EXT)
                {
                    temp->ARE = 01;
                    break;
                }
                else if (tempLabel->mainType == DECLERATION)
                {
                    temp->ARE = 10;
                    break;
                }
                tempLabel = searchListNode(tempLabel->next, temp->operandLabel[1], LABEL_TEXT);
            }
        }
        else
        {
            temp->ARE = 10;
        }
        temp = temp->next;
    }
}
int validateLabelList(lNode listHead, cNode headNode)
{
    lNode temp = listHead;
    lNode toCompare = NULL;

    while (temp != NULL)
    {
        if (temp->mainType == DECLERATION && temp->declarationError == 1)
        {
            toCompare = searchListNode(listHead, "", LINE_NUM); // return all
            while (toCompare != NULL)
            {
                if (strcmp(toCompare->name, temp->name) == 0)
                {
                    // checks if both same declerations and not the same line
                    if ((toCompare->lineNumber != temp->lineNumber) &&
                        (toCompare->mainType == DECLERATION))
                    {
                        printf("ERROR: in line %d - label '%s' has been declared more than once.\n", toCompare->lineNumber, temp->name);
                        temp->declarationError = -1;
                        toCompare->declarationError = -1;
                    }
                    // checks if declare and use
                    if (toCompare->mainType == USED_LABEL)
                    {
                        if (toCompare->lineNumber == temp->lineNumber)
                        {
                            printf("ERROR: in line %d - label '%s' is declared at the same line it is used.\n", toCompare->lineNumber, temp->name);
                            temp->declarationError = -1;
                            toCompare->declarationError = -1;
                        }
                        else
                        {
                            toCompare->declarationError = 1;
                            toCompare->address = temp->address;
                        }
                    }
                    // checks if no memory out of limits when using data[]
                    if (toCompare->type == DATA_LABEL)
                    {
                        if (temp->type != DATA_INT && temp->type != DATA_STRING && temp->type != EXT)
                        {
                            printf("ERROR: in line %d - label '%s' is not a .data or .string label.\n", toCompare->lineNumber, temp->name);
                            temp->declarationError = -1;
                            toCompare->declarationError = -1;
                        }
                        else
                        {
                            char tempStr[MAX_LABEL_SIZE * sizeof(int)];
                            sprintf(tempStr, "%d", temp->lineNumber);
                            cNode dataDeclare = searchNode(headNode, tempStr, LINE_NUM);
                            sprintf(tempStr, "%d", toCompare->lineNumber);
                            cNode dataUse = searchNode(headNode, tempStr, LINE_NUM);
                            int h = 0;
                            if (strcmp(dataUse->operandLabel[0], toCompare->name) == 0)
                            {
                                h = 0;
                            }
                            else if (strcmp(dataUse->operandLabel[1], toCompare->name) == 0)
                            {
                                h = 1;
                            }
                            if (((dataDeclare->numOfWords) > dataUse->operandImm[h]) || dataDeclare->lineType == EXT)
                            {
                                toCompare->declarationError = 1;
                                toCompare->address = temp->address;
                            }
                            else
                            {
                                printf("ERROR: in line %d - label '%s[%d]' is trying to accsess memory out of label declartion.\n", toCompare->lineNumber, temp->name, dataUse->operandImm[h]);
                                temp->declarationError = -1;
                                toCompare->declarationError = -1;
                            }
                        }
                    }
                }
                toCompare = toCompare->next;
            }
        }
        temp = temp->next;
    }

    temp = listHead;

    while (temp != NULL)
    {
        if (temp->declarationError < 1)
        {
            printf("ERROR: in line %d - label '%s' should be declared.\n", temp->lineNumber, temp->name);

            return false;
        }
        temp = temp->next;
    }
    return true;
}
void addLabelsAddressToCnode(cNode headNode, lNode labelList)
{
    cNode temp = headNode;
    while (temp != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            if (temp->operandType[i] == LABEL || temp->operandType[i] == DATA_LABEL)
            {
                lNode tempLabel = searchListNode(labelList, temp->operandLabel[i], LABEL_TEXT);
                while (tempLabel != NULL)
                {
                    if (tempLabel->mainType == DECLERATION)
                    {
                        temp->targetLabelAdd[i] = tempLabel->address;
                        break;
                    }
                    tempLabel = searchListNode(tempLabel->next, temp->operandLabel[i], LABEL_TEXT);
                }
            }
        }
        temp = temp->next;
    }
}
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr)
{
    cNode newTempNode;
    for (int i = 0; i < numOfAttr; i++)
    {
        cNode temp = orgListHead;
        while (temp != NULL)
        {
            lNode newListNode = (lNode)malloc(sizeof(list_object));
            newListNode->name[0] = '\0';
            newTempNode = searchNode(temp, "", listAttr[i]);

            if (newTempNode != NULL)
            {
                if (listAttr[i] == LABEL_TEXT && (newTempNode->labelText[0] != '\0'))
                {
                    strcpy(newListNode->name, newTempNode->labelText);
                    newListNode->declarationError = 1;
                    newListNode->mainType = DECLERATION;
                    newListNode->type = LABEL;
                    if (newTempNode->lineType == DATA_INT)
                    {
                        newListNode->type = DATA_INT;
                    }
                    if (newTempNode->lineType == DATA_STRING)
                    {
                        newListNode->type = DATA_STRING;
                    }
                    newListNode->address = newTempNode->lableAddressLine;
                }
                else if (
                    (listAttr[i] == OPERAND_TYPE_1) && ((newTempNode->operandType[0] == LABEL) || (newTempNode->operandType[0] == DATA_LABEL)))
                {
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = 0;
                    newListNode->mainType = USED_LABEL;
                    newListNode->type = OPERAND_LABEL_1;
                    if (newTempNode->operandType[0] == DATA_LABEL)
                    {
                        newListNode->type = DATA_LABEL;
                    }
                    newListNode->EXTUsedAddress = newTempNode->lableAddressLine + 1;
                }
                else if (
                    (listAttr[i] == OPERAND_TYPE_2) && ((newTempNode->operandType[1] == LABEL) || (newTempNode->operandType[1] == DATA_LABEL)))
                {
                    strcpy(newListNode->name, newTempNode->operandLabel[1]);
                    newListNode->declarationError = 0;
                    newListNode->mainType = USED_LABEL;
                    newListNode->type = OPERAND_LABEL_2;
                    if (newTempNode->operandType[1] == DATA_LABEL)
                    {
                        newListNode->type = DATA_LABEL;
                        newListNode->EXTUsedAddress = newTempNode->lableAddressLine + newTempNode->lineSize - 2;
                    }
                    else
                    {
                        newListNode->EXTUsedAddress = newTempNode->lableAddressLine + newTempNode->lineSize - 1;
                    }
                }
                else if ((listAttr[i] == LINE_TYPE) && (newTempNode->lineType == ENTRY))
                {
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = 0;
                    newListNode->mainType = USED_LABEL;
                    newListNode->type = ENTRY;
                }
                else if ((listAttr[i] == LINE_TYPE) && (newTempNode->lineType == EXT))
                {
                    strcpy(newListNode->name, newTempNode->operandLabel[0]);
                    newListNode->declarationError = 1;
                    newListNode->mainType = DECLERATION;
                    newListNode->type = EXT;
                }

                if (newListNode->name[0] != '\0')
                {
                    newListNode->next = newListHead;
                    newListHead = newListNode;
                    newListNode->lineNumber = newTempNode->lineNum;
                }
                temp = newTempNode->next;
            }
            else
            {
                temp = NULL;
            }
        }
    }
    return newListHead;
}
lNode searchListNode(lNode head, char *target, int searchAttr)
{
    lNode temp = head;
    while (temp != NULL)
    {
        switch (searchAttr)
        {
        case LINE_NUM:
            if (strcmp(target, "") == 0)
            {
                return temp;
            }
            else if (strcmp(target, "") != 0 && temp->lineNumber == atoi(target))
            {
                return temp;
            }
            temp = temp->next;
            continue;
        case LABEL_TEXT:
            if (strcmp(target, "") == 0 && (temp->name)[0] != '\0')
            {
                return temp;
            }
            else if (strcmp(target, "") != 0 && (strcmp(temp->name, target) == 0))
            {
                return temp;
            }
            temp = temp->next;
            continue;
        case MAIN_TYPE:
            if (strcmp(target, "") == 0 && temp->mainType > NA)
            {
                return temp;
            }
            else if (strcmp(target, "") != 0 && temp->mainType == atoi(target))
            {
                return temp;
            }
            temp = temp->next;
            continue;

        case TYPE:
            if (strcmp(target, "") == 0 && temp->type > NA)
            {
                return temp;
            }
            else if (strcmp(target, "") != 0 && temp->type == atoi(target))
            {
                return temp;
            }
            temp = temp->next;
            continue;
        default:
            printf("ERROR: searchListNode: '%d' is not a valid attribute for search\n", searchAttr);
            return NULL;
            continue;
        }
    }
    printf("ERROR: searchListNode: '%s' not found\n", target);
    return NULL;
}
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
void freeAll(cNode headNode, lNode labelList)
{
    cNode temp = headNode;
    while (temp != NULL)
    {
        headNode = temp->next;
        if (temp->dataArray != NULL)
        {
            free(temp->dataArray);
        }
        free(temp);
        temp = headNode;
    }
    lNode tempL = labelList;
    while (tempL != NULL)
    {
        labelList = tempL->next;
        free(tempL);
        tempL = labelList;
    }
}
