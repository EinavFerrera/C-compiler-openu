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
    // printLoop(headNode);
    printf("\t\t\t\t\t\t\t\t\t************ code to nodes finished for file %s! \n", fileName);
    if (searchNode(headNode, "", ERROR_COUNT) != NULL)
    {
        printf("\t\t\t\t\t\t\t\t\t************ There are errors in the file %s! \n", fileName);
        return;
    }
    // checks if label are valid - declared and used, no double declerationsm no data label out of limits
    lNode labelList = NULL;
    int srcList[] = {LABEL_TEXT, OPERAND_TYPE_1, OPERAND_TYPE_2, LINE_TYPE};
    int srcListLenght = sizeof(srcList) / sizeof(srcList[0]);
    labelList = createLabelsList(headNode, labelList, srcList, srcListLenght);
    validateLabelList(labelList, headNode);
    printf("\t\t\t\t\t\t\t\t\t************ label list is valid\n");

    if (searchListNode(labelList, "20", TYPE) != NULL)
    {
        printf("\t\t\t\t\t\t\t\t\t************ entry label created\n");
        createEntryExternFile(labelList, ENTRY);
    }
    if (searchListNode(labelList, "21", TYPE) != NULL)
    {
        createEntryExternFile(labelList, EXT);
        printf("\t\t\t\t\t\t\t\t\t************ extern Label created\n");
    }
    // checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01
    AREAssign(headNode, labelList);
    printf("\t\t\t\t\t\t\t\t\t************ ARE assigned\n");
    numOfLinesAssign(headNode);

    // printLoop(headNode);

    fclose(inputFile);
}
// check the count of par, thier types and locations are valid for the CODE, add numOFLines to the node
// data lable = 2 words in memory ( one for the label and one for the imm)
void createEntryExternFile(lNode labelList, int searchAttr)
{
}
void numOfLinesAssign(cNode headNode)
{
}

void AREAssign(cNode headNode, lNode labelList)
{
    // checks if type oparnd: both imm/reg = 00, local label = 10 , external label = 01
    cNode temp = headNode;
    while (temp != NULL)
    {
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
        else if (temp->operandType[0] == LABEL)
        {
            lNode tempLabel = searchListNode(labelList, temp->operandLabel[0], LABEL_TEXT);
            while (tempLabel != NULL)
            {
                if (tempLabel->type == EXT)
                {
                    temp->ARE = 01;
                }
                tempLabel = searchListNode(tempLabel->next, temp->operandLabel[0], LABEL_TEXT);
            }
        }
        else if (temp->operandType[1] == LABEL)
        {
            lNode tempLabel = searchListNode(labelList, temp->operandLabel[1], LABEL_TEXT);
            while (tempLabel != NULL)
            {
                if (tempLabel->type == EXT)
                {
                    temp->ARE = 01;
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

    lNode temp = listHead;  // 1
    lNode toCompare = NULL; // A,B,C...
    while (temp != NULL)    // 1,2,3...
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
                        }
                    }
                    // checks if no memory out of limits when using data[]
                    if (toCompare->type == DATA_LABEL)
                    {
                        if (temp->type != DATA_INT && temp->type != DATA_STRING)
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
                            if ((dataDeclare->numOfWords) >
                                dataUse->operandImm[1]) // can be operandImm[0]??
                            {
                                toCompare->declarationError = 1;
                            }
                            else
                            {
                                printf("ERROR: in line %d - label '%s[%d]' is trying to accsess memory out of label declartion.\n", toCompare->lineNumber, temp->name, dataUse->operandImm[1]);
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
        if (temp->declarationError == 0)
        {
            printf("ERROR: in line %d - label '%s' should be declared.\n", temp->lineNumber, temp->name);
        }
        temp = temp->next;
    }
    temp = listHead;

    while (temp != NULL)
    {
        if (temp->declarationError < 1)
        {
            return false;
        }
        temp = temp->next;
    }
    return true;
}
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr)
{
    cNode newTempNode = (cNode)calloc(1, sizeof(contentNode_object));
    for (int i = 0; i < numOfAttr; i++)
    {
        cNode temp = orgListHead;
        while (temp != NULL)
        {
            lNode newListNode = (lNode)calloc(1, sizeof(list_object));
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
            }
            temp = newTempNode->next;
        }
    }
    return newListHead;
}
lNode searchListNode(lNode head, char *target, int srcAttr)
{
    lNode temp = head;
    while (temp != NULL)
    {
        switch (srcAttr)
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
            printf("ERROR: searchListNode: '%d' is not a valid attribute for search\n", srcAttr);
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