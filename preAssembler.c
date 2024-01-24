#include "genericH.h"

void preAssembel(char *fileName)
{
    /*
     open file with am to write
     open file with as to read
     pass 1:
        read line by line from AS
        replace all comments and spaces
     pass 2:
        read line by line from AM
        replace all macros
     pass 3:
        read line by line from AM
        replace all defines
            read line by line
            if line is a macro
                read macro
                replace macro with macro content
            if line is a define
                add define to defines array
            if line is already defined
                show error

    */
    /*var decleration section*/
    char fileNameAs[MAX_LABEL_SIZE];
    char fileNameAm[MAX_LABEL_SIZE];
    FILE *asFile;
    FILE *amFile;
    char line[MAX_LINE_SIZE] = {0};
    int lineCounter;
    /*------------------------*/
    lineCounter = 0;

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

    while (fgets(line, sizeof(line), asFile))
    {
        if (removeCommentsAndSpaces(line)) /*if comments return 1, or replace line with single space only*/
        {
            continue;
        }
        //     if (replaceDefines(line))
        //     { /*if define return 1 its an error*/
        //         return;
        //     }
        //     fprintf(amFile, "%s", line);
        //     memset(line, 0, sizeof(line));
        // }
        // if (processMacro(amFile))
        // {
        //     return;
        // }
        printf("%s", line);
    }

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 100
#define MAX_DEFINES 100 /*temp so i can make it work*/

    int processMacro(FILE * inputfile)
    // int processMacro(char *input[], int *lineCount)
    {
        int i;
        int j;
        char *macroName = NULL;
        char *macroContents[MAX_LINES];
        int macroLineCount = 0;

        for (i = 0; i < *lineCount; i++)
        {
            if (strncmp(input[i], "mcr", 3) == 0)
            {
                macroName = strtok(input[i] + 4, " ");
                i++; /*Skip the mcr line*/
                while (strncmp(input[i], "endmcr", 6) != 0)
                {
                    macroContents[macroLineCount++] = input[i++];
                }
            }
            else if (macroName != NULL && strncmp(input[i], macroName, strlen(macroName)) == 0)
            {
                /*Replace macro invocation with macroContents*/
                for (j = 0; j < macroLineCount; j++)
                {
                    input[*lineCount + j] = macroContents[j];
                }
                *lineCount += macroLineCount;
                macroName = NULL; /*Reset macroName after processing*/
            }
        }
    }

    typedef struct
    {
        char name[MAX_LABEL_SIZE]; /* array of defines*/
        int value;                 /*define's value*/
        int defined;               /*the flag indicate if the define is already exists*/
    } Define;

    Define defines[MAX_DEFINES] = {0};

    int getDefineIndex(char *name)
    {
        int i;
        for (i = 0; i < MAX_DEFINES; i++)
        {
            if (strcmp(defines[i].name, name) == 0 && defines[i].defined)
            {
                return i;
            }
        }
        return -1;
    }
    int replaceDefines(char *text)
    {
        int len = strlen(text);
        int i;
        int defineIndex;
        int nameStart;
        int nameEnd;
        int equalsPos;

        int j = 0;

        for (i = 0; i < len; i++)
        {
            if (text[i] == '.' && i + 7 < len && strncmp(&text[i], ".define", 7) == 0)
            {
                nameStart = i + 8;
                /* Find the end of the define name*/
                nameEnd = nameStart;
                while (isalpha(text[nameEnd]))
                {
                    nameEnd++;
                }

                if (nameStart < nameEnd)
                {
                    /*Extract the define name*/
                    char name[MAX_LABEL_SIZE];
                    strncpy(name, &text[nameStart], nameEnd - nameStart);
                    name[nameEnd - nameStart] = '\0';

                    /*Find the position of '='*/
                    equalsPos = nameEnd;
                    while ((text[equalsPos]) != 32 && (text[equalsPos]) != 9) /*32 = space, 9 = tab*/
                    {
                        equalsPos++;
                    }

                    if (text[equalsPos] == '=')
                    {
                        /*Skip whitespace after '='*/
                        equalsPos++;
                        while ((text[equalsPos]) != 32 && (text[equalsPos]) != 9)
                        {
                            equalsPos++;
                        }

                        defineIndex = getDefineIndex(name);

                        if (defineIndex == -1)
                        {
                            /*Define not found, add it*/
                            if (!defines[j].defined)
                            {
                                strcpy(defines[j].name, name);
                                char tempNumber[MAX_LINE_LENGTH];
                                sscanf(&text[equalsPos], "%s", tempNumber);
                                if (!isOnlyDigit(tempNumber))
                                {
                                    printf("Error: Define '%s' is not a number.\n", name);
                                    break;
                                }
                                else
                                {
                                    defines[j].value = atoi(tempNumber);
                                    defines[j].defined = 1;
                                    j++;
                                }
                            }
                        }
                        else
                        {
                            /*Define already exists, show an error*/
                            printf("Error: Define '%s' redefined.\n", name);
                            break;
                        }
                    }
                }
            }
        }
        if (isalpha(text[i])) /*temp so i can make it work*/
        {
            /*Find the end of the define name*/
            nameEnd = i;
            while (isalpha(text[nameEnd]))
            {
                nameEnd++;
            }

            if (i < nameEnd)
            {
                /*Extract the define name*/
                char name[MAX_LINE_LENGTH];
                strncpy(name, &text[i], nameEnd - i);
                name[nameEnd - i] = '\0';

                defineIndex = getDefineIndex(name);

                if (defineIndex != -1)
                {
                    /*Replace the define with its value*/
                    char valueStr[MAX_LINE_LENGTH];
                    sprintf(valueStr, "%d", defines[defineIndex].value);
                    memmove(&text[i + strlen(valueStr)], &text[nameEnd], len - nameEnd + 1);
                    strncpy(&text[i], valueStr, strlen(valueStr));
                    len += strlen(valueStr) - 1;
                }
            }
        }
    }
    int removeCommentsAndSpaces(char *line)
    {
        if (line[0] == ';')
        {
            return TRUE;
        }
        char newLine[MAX_LINE_LENGTH] = {'\0'};
        clearDuplicateBlankChars(line, newLine);
        memset(line, 0, MAX_LINE_LENGTH);
        strcpy(line, newLine);
        return FALSE;
    }