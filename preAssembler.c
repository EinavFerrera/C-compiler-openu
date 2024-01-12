#include "genericH.h"

void fileCompiling(char *fileName)
{
    // Create the output file name
    char outputFileName[MAX_LINE_SIZE];
    strcpy(outputFileName, fileName);
    strcat(outputFileName, ".am");

    // Open the input file
    char fileNameAs[MAX_LINE_SIZE];
    strcpy(fileNameAs, fileName);
    strcat(fileNameAs, ".as");
    FILE *inputFile = fopen(fileNameAs, "r");
    if (inputFile == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    // Open the output file
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL)
    {
        printf("Failed to create the output file.\n");
        fclose(inputFile);
        return;
    }

    // clear spaces and tabs
    // clear notes
    // extractMcr(inputFile, outputFile);
    // replace defines

    fclose(inputFile);
    fclose(outputFile);
    return;
}

// void extractMcr(FILE *inputFile, FILE *outputFile)
// {
//     char line[MAX_LINE_SIZE]; // what is the for each line?
//     while (fgets(line, sizeof(line), inputFile))
//     {
//         if (searchMcr == MCR)
//         {
//             char *mcrContent = (char *)malloc(1);
//             char *mcrName = (char *)malloc(sizeof(char) * sizeof(line - 4));
//             strncpy(mcrName, line + 4, sizeof(line - 4));
//             while (fgets(line, sizeof(line), inputFile))
//             {
//                 if (searchMcr == ENDMCR)
//                 {
//                     break;
//                 }
//                 else
//                 {
//                     mcrContent = (char *)realloc(mcrContent, sizeof(mcrContent) + sizeof(line));
//                     strcat(mcrContent, line);
//                 }
//             }
//             createNewMcrNode(mcrName, mcrContent);
//         }
//         else if (searchMcr == NOMCR)
//         {
//             fputs(line, outputFile);
//         }
//         // Write the modified line to the output file
//         fputs(line, outputFile);
//     }
// }

// int searchMcr(char *startOfLine, bool *isMcr)
// {
//     char mcr[] = "mcr";
//     char endmcr[] = "endmcr";

//     if (((strncmp(startOfLine, "mcr", 3)) == 0) && ((*(startOfLine + 3) == '\t') || (*(startOfLine + 3) == ' '))) /*search for 'mcr'*/
//     {
//         *isMcr = true;
//         return MCR;
//     }
//     else if (((strncmp(startOfLine, "endmcr", 6)) == 0) && ((*(startOfLine + 6) == '\t') || (*(startOfLine + 6) == '\r') || (*(startOfLine + 6) == '\n') || (*(startOfLine + 6) == ' '))) /*search for 'endmcr'*/
//     {
//         // needs to clear all the spaces
//         *isMcr = false;
//         return ENDMCR;
//     }
//     else /*no MCR in line*/
//     {
//         return NOMCR;
//     }
// }
// void replaceMacro(char *line)
// {
// }

// //     int index = 1;
// // 	int start, end;
// //     char *ptr;
// // 	char filePath[MAXIMUM_FILE_NAME];
// //     char line[LINE_LEN];
// // 	FILE *original;
// // 	sprintf(filePath,"%s.as",*(fileName));
// // 	original = fopen(filePath, "r");
// // 	start = (int)getStartMacro(node);
// // 	end = (int)getEndMacro(node);

// //     while (fgets(line, sizeof(line), original) != NULL)
// // 	{
// // 		if (index < start)
// // 		{
// // 			index += 1;
// // 		}
// // 		else
// // 		{
// // 			if (start <= end)
// // 			{
// // 				ptr = (char *)&line;
// // 				ignoreSpaceTab(&ptr);
// // 				fputs(ptr, *modified);
// // 				start += 1;
// // 				i = start;
// // 			}
// // 			else
// // 				break;
// // 		}
// // 	}
// // 	fclose(original);
// // }

#define MAX_DEFINES 100
#define MAX_LINE_LENGTH 1000

typedef struct {
    char name[MAX_LINE_LENGTH];
    int value;
    int defined;
} Define;

Define defines[MAX_DEFINES] = {0};

int getDefineIndex(char *name) {
    for (int i = 0; i < MAX_DEFINES; i++) {
        if (strcmp(defines[i].name, name) == 0 && defines[i].defined) {
            return i;
        }
    }
    return -1;
}

void replaceDefines(char *text) {
    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        if (text[i] == '.' && i + 7 < len && strncmp(&text[i], ".define", 7) == 0) {
            int nameStart = i + 8;

            // Find the end of the define name
            int nameEnd = nameStart;
            while (isalpha(text[nameEnd])) {
                nameEnd++;
            }

            if (nameStart < nameEnd) {
                // Extract the define name
                char name[MAX_LINE_LENGTH];
                strncpy(name, &text[nameStart], nameEnd - nameStart);
                name[nameEnd - nameStart] = '\0';

                int defineIndex = getDefineIndex(name);

                if (defineIndex == -1) {
                    // Define not found, add it
                    for (int j = 0; j < MAX_DEFINES; j++) {
                        if (!defines[j].defined) {
                            strcpy(defines[j].name, name);
                            sscanf(&text[nameEnd + 1], "%d", &defines[j].value);
                            defines[j].defined = 1;
                            break;
                        }
                    }
                } else {
                    // Define already exists, show an error
                    printf("Error: Define '%s' redefined.\n", name);
                    exit(EXIT_FAILURE);
                }

            }
        } else if (isalpha(text[i])) {
            // Find the end of the define name
            int nameEnd = i;
            while (isalpha(text[nameEnd])) {
                nameEnd++;
            }

            if (i < nameEnd) {
                // Extract the define name
                char name[MAX_LINE_LENGTH];
                strncpy(name, &text[i], nameEnd - i);
                name[nameEnd - i] = '\0';

                int defineIndex = getDefineIndex(name);

                if (defineIndex != -1) {
                    // Replace the define with its value
                    char valueStr[MAX_LINE_LENGTH];
                    sprintf(valueStr, "%d", defines[defineIndex].value);
                    memmove(&text[i + strlen(valueStr)], &text[nameEnd], len - nameEnd + 1);
                    strncpy(&text[i], valueStr, strlen(valueStr));
                    len += strlen(valueStr) - 1;
                }
            }
        }
    }
}

