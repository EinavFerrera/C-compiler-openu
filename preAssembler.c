#include "genericH.h"

int preAssembel(char *fileName)
{
    /*var decleration section*/
    char fileNameAs[MAX_LABEL_SIZE];
    char fileNameAm[MAX_LABEL_SIZE];
    FILE *asFile;
    FILE *amFile;
    char line[MAX_LINE_SIZE] = {0};
    int lineCounter;
    int isDefineLine = 0;
    int defineCounter = 0;
    dNode defines = NULL;

    /*------------------------*/
    lineCounter = 1;

    /* Open the src file*/
    strcpy(fileNameAs, fileName);
    strcat(fileNameAs, ".as");
    asFile = fopen(fileNameAs, "r");
    if (asFile == NULL)
    {
        printf("Failed to open the file \"%s\".\n", fileName);
        return TRUE;
    }
    /* Open the dst file*/
    strcpy(fileNameAm, fileName);
    strcat(fileNameAm, ".am");
    amFile = fopen(fileNameAm, "w");
    if (amFile == NULL)
    {
        printf("Failed to create new AM file \"%s\".\n", fileName);
        return TRUE;
    }

    defines = (dNode)malloc(sizeof(Define));
    memset(defines->name, 0, MAX_LABEL_SIZE);
    defines->value = NA;
    defines->defined = FALSE;
    defines->next = NULL;

    while (fgets(line, sizeof(line), asFile))
    {
        removeCarrigeReturn(line);
        if (removeCommentsAndSpaces(line)) /*if comment return 1, or replace line with single space only*/
        {
            continue;
        }
        isDefineLine = replaceDefines(line, lineCounter, &defineCounter, &defines);

        if (isDefineLine == 1)
        { /*if define return 1 its an error, if returns 0 = skip define line*/
            freeDefines(defines);
            return TRUE;
        }
        else if (isDefineLine == 0)
        {
            memset(line, 0, sizeof(line));
            continue;
        }
        fprintf(amFile, "%s", line);
        memset(line, 0, sizeof(line));
        lineCounter++;
    }

    freeDefines(defines);
    fclose(asFile);
    fclose(amFile);
    if (processMacro(fileName))
    {
        printf("\t\t\t************ Failed to process macro in file \"%s\" ************\n", fileName);
        return TRUE;
    }
    return FALSE;
}

int processMacro(char *fileName)
{
    char macroName[MAX_LABEL_SIZE] = {0};
    char *macroContents;
    char *tempAmFile;
    char fileNameAm[MAX_LABEL_SIZE];
    FILE *amFile;
    char line[MAX_LINE_SIZE] = {0};
    int foundMacro = FALSE;
    do
    {
        /* Open the src file*/
        strcpy(fileNameAm, fileName);
        strcat(fileNameAm, ".am");
        amFile = fopen(fileNameAm, "r");
        if (amFile == NULL)
        {
            printf("Failed to create new AM file \"%s\".\n", fileName);
            return TRUE;
        }

        foundMacro = FALSE;
        macroContents = (char *)malloc(MAX_LINE_SIZE * sizeof(char));
        macroContents[0] = '\0';
        tempAmFile = (char *)malloc(MAX_LINE_SIZE * sizeof(char));
        tempAmFile[0] = '\0';
        while (fgets(line, sizeof(line), amFile))
        {
            if (macroName[0] == 0 && strncmp(line, "mcr", 3) == 0)
            {
                foundMacro = TRUE;
                strncpy(macroName, line + 4, strlen(line + 4));
                macroName[strlen(macroName) - 1] = '\0';
                macroName[strlen(macroName)] = '\0';
                if (savedWord(macroName))
                {
                    printf("Error: Macro name cannot be a saved word in the line: %s", line);
                    return TRUE;
                }
                if (hasSpacesInWord(macroName))
                {
                    printf("Error: Macro name cannot contain more than one label in the line: %s", line);
                    return TRUE;
                }
                clearAllBlanks(macroName);
                while (fgets(line, sizeof(line), amFile))
                {
                    if (strncmp(line, "endmcr", 6) == 0)
                    {
                        break;
                    }
                    else
                    {
                        macroContents = (char *)realloc(macroContents, (1 + strlen(macroContents) + strlen(line)) * sizeof(char));
                        strcat(macroContents, line);
                    }
                }
            }
            else if (macroName[0] != 0 &&
                     strlen(line) >= strlen(macroName) &&
                     (strncmp(line, macroName, strlen(macroName)) == 0) &&
                     (!isalpha(line[strlen(macroName)]) ||
                      line[strlen(macroName)] == ' ' ||
                      line[strlen(macroName)] == '\t' ||
                      line[strlen(macroName)] == '\n' ||
                      line[strlen(macroName)] == '\r' ||
                      line[strlen(macroName)] == '\0'))
            {
                tempAmFile = (char *)realloc(tempAmFile, (1 + strlen(tempAmFile) + strlen(macroContents)) * sizeof(char));
                strcat(tempAmFile, macroContents);
            }
            else
            {
                tempAmFile = (char *)realloc(tempAmFile, (1 + strlen(tempAmFile) + strlen(line)) * sizeof(char));
                strcat(tempAmFile, line);
            }
        }
        fclose(amFile);

        /* Open the dst file*/
        strcpy(fileNameAm, fileName);
        strcat(fileNameAm, ".am");
        amFile = fopen(fileNameAm, "w");
        if (amFile == NULL)
        {
            printf("Failed to create new AM file \"%s\".\n", fileName);
            return TRUE;
        }

        fprintf(amFile, "%s", tempAmFile);
        free(macroContents);
        free(tempAmFile);
        memset(macroName, 0, sizeof(macroName));
        fclose(amFile);
    } while (foundMacro);
    return FALSE;
}

dNode getDefineIndex(char *name, int defineCounter, dNode defines)
{
    dNode temp = defines;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0 && temp->defined == TRUE)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
int replaceDefines(char *text, int lineNum, int *defineCounter, dNode *defines)
{
    int length = strlen(text);
    char name[MAX_LABEL_SIZE];
    char textNum[MAX_LINE_SIZE];
    int i = 0;
    int p = 0;
    int j = 0;
    dNode defineNode = NULL;

    char tempName[MAX_LABEL_SIZE];
    char tempNum[MAX_LINE_SIZE];
    int hasSpace = FALSE;
    while (text[i] != '\0')
    {
        if (i == 0 && length > 8 && strncmp(&text[i], ".define", 7) == 0 && text[i + 7] == ' ')
        { /*legit define*/
            i += 8;
            while (text[i] != '\0' && text[i] != ' ' && text[i] != '=')
            {
                if (text[i] < 97 || text[i] > 122)
                {
                    printf("Error: In line %d - Define has no valid define label in line : %s", lineNum, text);
                    return TRUE;
                }
                name[j] = text[i];
                i++;
                j++;
            }
            name[j] = '\0';
            if (text[i] == '\0')
            {
                printf("Error: In line %d - Define has no valid define label in line: %s", lineNum, text);
                return TRUE;
            }
            if (text[i] == ' ')
            {
                i++;
            }
            if (text[i] == '=')
            {
                i++;
                if (text[i] == ' ')
                {
                    i++;
                }
                while (text[i] != '\0' && text[i] != '\n' && text[i] != '\r' && text[i] != ' ' && text[i] != '\t')
                {
                    textNum[p] = text[i];
                    p++;
                    i++;
                }
                textNum[p] = '\0';

                if (savedWord(name))
                {
                    printf("Error: In line %d - Define cannot be a saved word in the line: %s", lineNum, text);
                    return TRUE;
                }
                if (!isOnlyDigit(textNum))
                {
                    printf("Error: In line %d - Define value is not a number in line: %s", lineNum, text);
                    return TRUE;
                }
                if (getDefineIndex(name, *defineCounter, *defines) != NULL)
                {
                    printf("Error: In line %d - Define label already exists", lineNum);
                    return TRUE;
                }

                while (text[i] == ' ' || text[i] == '\t')
                {
                    i++;
                }

                if (text[i] != '\0' && text[i] != '\n' && text[i] != '\r')
                {
                    printf("Error: In line %d - Define decleration contains more than one number in line: %s", lineNum, text);
                    return TRUE;
                }
                else
                {
                    dNode newDefine = (dNode)malloc(sizeof(Define));
                    strcpy(newDefine->name, name);
                    newDefine->value = atoi(textNum);
                    newDefine->defined = TRUE;
                    newDefine->next = *defines;
                    *defines = newDefine;
                    *defineCounter = *defineCounter + 1;
                    return FALSE;
                }
            }
            else
            {
                printf("Error: In line %d - Define decleration does not contain number in line: %s", lineNum, text);
                return TRUE;
            }
        }
        else
        {
            while (text[i] != '\0')
            {
                if (text[i] == ' ' ||
                    text[i] == ',' ||
                    text[i] == '#')
                {
                    hasSpace = TRUE;
                    i++;
                    if (text[i - 1] == ',' && text[i] == ' ')
                    {
                        i++;
                    }
                }

                while (isalpha(text[i]))
                {
                    tempName[j] = text[i];
                    i++;
                    j++;
                }
                if (text[i] == ' ' || text[i] == ',' || text[i] == '\n' || text[i] == 't' ||
                    text[i] == '\0' ||
                    (text[i] == ']' && text[i - j - 1] == '['))
                {
                    hasSpace = TRUE;
                }
                tempName[j] = '\0';
                defineNode = getDefineIndex(tempName, *defineCounter, *defines);
                if (defineNode != NULL && hasSpace)
                {
                    sprintf(tempNum, "%d", defineNode->value);
                    j = strlen(tempNum); /*save the length f the new number*/
                    strncat(tempNum, &text[i], strlen(&text[i] - 1));
                    memmove(&text[i - strlen(tempName)], &tempNum, strlen(tempNum) + 1);
                    i = i - strlen(tempName) + j;
                }
                j = 0;
                i++;
                hasSpace = FALSE;
                tempName[0] = '\0';
                tempNum[0] = '\0';
            }
        }
        i++;
    }
    return 2;
}
int removeCommentsAndSpaces(char *line)
{
    int i = 0;
    char newLine[MAX_LINE_SIZE] = {'\0'};
    if (line[0] == ';')
    {
        return TRUE;
    }
    for (i = 7; i < strlen(line); i++)
    {
        if (strncmp(&line[i - 7], ".string", 7) == 0)
        {
            return FALSE;
        }
    }
    clearDuplicateBlankChars(line, newLine);
    memset(line, 0, MAX_LINE_SIZE);
    strcpy(line, newLine);
    return FALSE;
}
int savedWord(char *textNum)
{
    if (strcmp(textNum, "mcr") == 0 ||
        strcmp(textNum, "endmcr") == 0 ||
        strcmp(textNum, "mov") == 0 ||
        strcmp(textNum, "cmp") == 0 ||
        strcmp(textNum, "add") == 0 ||
        strcmp(textNum, "sub") == 0 ||
        strcmp(textNum, "lea") == 0 ||
        strcmp(textNum, "clr") == 0 ||
        strcmp(textNum, "not") == 0 ||
        strcmp(textNum, "inc") == 0 ||
        strcmp(textNum, "dec") == 0 ||
        strcmp(textNum, "jmp") == 0 ||
        strcmp(textNum, "bne") == 0 ||
        strcmp(textNum, "red") == 0 ||
        strcmp(textNum, "prn") == 0 ||
        strcmp(textNum, "jsr") == 0 ||
        strcmp(textNum, "rts") == 0 ||
        strcmp(textNum, "hlt") == 0 ||
        strcmp(textNum, "r0") == 0 ||
        strcmp(textNum, "r1") == 0 ||
        strcmp(textNum, "r2") == 0 ||
        strcmp(textNum, "r3") == 0 ||
        strcmp(textNum, "r4") == 0 ||
        strcmp(textNum, "r5") == 0 ||
        strcmp(textNum, "r6") == 0 ||
        strcmp(textNum, "r7") == 0 ||
        strcmp(textNum, "data") == 0 ||
        strcmp(textNum, "string") == 0 ||
        strcmp(textNum, "entry") == 0 ||
        strcmp(textNum, "extern") == 0)
    {
        return TRUE;
    }
    return FALSE;
}
void freeDefines(dNode defines)
{
    dNode temp = defines;
    while (temp != NULL)
    {
        defines = defines->next;
        free(temp);
        temp = defines;
    }
}
