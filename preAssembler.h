#ifndef preAssembler
#define preAssembler

/**
 * Compiles the given file.
 *
 * @param fileName The name of the file to be compiled.
 */
void fileCompiling(char *fileName);

void extractMcr(FILE *inputFile, FILE *outputFile);

int searchMcr(char *startOfLine, bool *isMcr);

#endif
