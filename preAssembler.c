#include "genericH.h"

void fileCompiling(char *fileName)
{
    char outputFileName[MAX_LINE_SIZE];
    char fileNameAs[MAX_LINE_SIZE];
    FILE *inputFile;
    FILE *outputFile;
    /*Create the output file name*/
    strcpy(outputFileName, fileName);
    strcat(outputFileName, ".am");

    /*Open the input file*/
    strcpy(fileNameAs, fileName);
    strcat(fileNameAs, ".as");
    inputFile = fopen(fileNameAs, "r");
    if (inputFile == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    /*Open the output file*/
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL)
    {
        printf("Failed to create the output file.\n");
        fclose(inputFile);
        return;
    }

    fclose(inputFile);
    fclose(outputFile);
    return;
}

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 100
#define MAX_DEFINES 100 /*temp so i can make it work*/

void processMacro(char *input[], int *lineCount);
void processMacro(char *input[], int *lineCount)
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
    char name[MAX_LINE_LENGTH]; /* array of defines*/
    int value;                  /*define's value*/
    int defined;                /*the flag indicate if the define is already exists*/
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

void replaceDefines(char *text)
{
    int len = strlen(text);
    int i;
    int defineIndex;
    int nameStart;
    int nameEnd;
    int equalsPos;

    int j = 0; /*temp so i can make*/

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
                char name[MAX_LINE_LENGTH];
                strncpy(name, &text[nameStart], nameEnd - nameStart);
                name[nameEnd - nameStart] = '\0';

                /*Find the position of '='*/
                equalsPos = nameEnd;
                while (isspace(text[equalsPos]))
                {
                    equalsPos++;
                }

                if (text[equalsPos] == '=')
                {
                    /*Skip whitespace after '='*/
                    equalsPos++;
                    while (isspace(text[equalsPos]))
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
                            sscanf(&text[equalsPos], "%d", &defines[j].value);
                            defines[j].defined = 1;
                            break;
                        }
                    }
                }
                else
                {
                    /*Define already exists, show an error*/
                    printf("Error: Define '%s' redefined.\n", name);
                    exit(EXIT_FAILURE);
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
