#ifndef preAssembler
#define preAssembler

typedef struct
{
    char name[MAX_LABEL_SIZE]; /* array of defines*/
    int value;                 /*define's value*/
    int defined;               /*the flag indicate if the define is already exists*/
} Define;

/**
 * Compiles the given file.
 *
 * @param fileName The name of the file to be compiled.
 * @return 1 if found an error, 0 if success.
 */
int preAssembel(char *fileName);

/**
 * Processes the macro in the input file.
 *
 * @param fileName The input file name to process.
 * @return 0 if successful, -1 otherwise.
 */
int processMacro(char *fileName);

/**
 * Gets the index of the define with the given name.
 *
 * @param name The name of the define.
 * @param defineCounter The number of defines already made.
 * @return The index of the define if found, -1 otherwise.
 */
int getDefineIndex(char *name, int defineCounter, Define *defines);

/**
 * Replaces the defines in the given text.
 *
 * @param text The text to replace defines in.
 * @param lineNum The line number of the text.
 * @param defineCounter The number of defines already made.
 * @return 1 if found an error, 0 if found a define decleration, 2 if found an define use that have been replaced.
 */
int replaceDefines(char *text, int lineNum, int *counter, Define *defines);

/**
 * Removes comments and spaces from the given line.
 *
 * @param line The line to remove comments and spaces from.
 * @return 1 if a coment, 0 otherwise.
 */
int removeCommentsAndSpaces(char *line);

/**
 *checks if it is a saved word.
 *
 * @param token the word to comapre to the saved words.
 * @return 1 if is a saved word, 0 otherwise.
 */
int savedWord(char *token);

/**
 * S.
 *
 * @param defines pointer to the array.
 * @param defineCounter the index in the array to initiate.
 */
void initDefine(Define *defines, int defineCounter);

#endif
