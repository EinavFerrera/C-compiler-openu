#include "preAssembler.h"
#include "genericH.h"

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

    // Read line by line from the input file
    char line[1000];//what is the for each line?
    while (fgets(line, sizeof(line), inputFile)) {
        // Append "**" to the line
        strcat(line, "**!");

        // Write the modified line to the output file
        fputs(line, outputFile);
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);
}
    