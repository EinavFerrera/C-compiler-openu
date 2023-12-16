#include "genericH.h"
#include "preAssembler.h"


void fileCompiling(char* fileName) {
    // Create the output file name
    char outputFileName[100];
    strcpy(outputFileName, fileName);
    strcat(outputFileName, ".am");

    // Open the input file
    strcat(fileName, ".as");
    FILE* inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    // Open the output file
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Failed to create the output file.\n");
        fclose(inputFile);
        return;
    }
    extractMcr(inputFile, outputFile);

    // // Read line by line from the input file
    // char line[MAX_LINE_SIZE];//what is the for each line?
    // while (fgets(line, sizeof(line), inputFile)) {
    //     // Append "**" to the line
    //     strcat(line, "**!");

    //     // Write the modified line to the output file
    //     fputs(line, outputFile);
    // }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);
}
void extractMcr(FILE *inputFile, FILE *outputFile)
{
    char line[MAX_LINE_SIZE];//what is the for each line?
    while (fgets(line, sizeof(line), inputFile)) {
        // Append "**" to the line
        strcat(line, "**!");

        // Write the modified line to the output file
        fputs(line, outputFile);
    }
    
    
}

int srcMcr(char *startOfLine, bool *isMcr)
{
	char mcr[] = "mcr";
	char endmcr[] = "endmcr";

	if (((strncmp(startOfLine, mcr, 3)) == 0) && ((*(startOfLine + 3) == '\t') || (*(startOfLine + 3) == ' '))) /*search for 'mcr'*/
	{
		*isMcr = true;
		return MCR;
	}
	else if (((strncmp(startOfLine, endmcr, 6)) == 0) && ((*(startOfLine + 6) == '\t') || (*(startOfLine + 6) == '\r') || (*(startOfLine + 6) == '\n') || (*(startOfLine + 6) == ' '))) /*search for 'endmcr'*/
	{
		*isMcr = false;
		return ENDMCR;
	}
	else /*no MCR in line*/
	{
		return NOMCR;
	}
}   
    
    
    
    
    
    
    
    
    
    
    
    
    
//     int index = 1;
// 	int start, end;
//     char *ptr;
// 	char filePath[MAXIMUM_FILE_NAME];
//     char line[LINE_LEN];
// 	FILE *original;
// 	sprintf(filePath,"%s.as",*(fileName));
// 	original = fopen(filePath, "r");
// 	start = (int)getStartMacro(node);
// 	end = (int)getEndMacro(node);

//     while (fgets(line, sizeof(line), original) != NULL)
// 	{
// 		if (index < start)
// 		{
// 			index += 1;
// 		}
// 		else
// 		{
// 			if (start <= end)
// 			{
// 				ptr = (char *)&line;
// 				ignoreSpaceTab(&ptr);
// 				fputs(ptr, *modified);
// 				start += 1;
// 				i = start;
// 			}
// 			else
// 				break;
// 		}
// 	}
// 	fclose(original);
// }


    