#ifndef preAssembler
#define preAssembler

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
 * @param inputfile The input file to process.
 * @return 0 if successful, -1 otherwise.
 */
int processMacro(FILE *inputfile);

/**
 * Gets the index of the define with the given name.
 *
 * @param name The name of the define.
 * @return The index of the define if found, -1 otherwise.
 */
int getDefineIndex(char *name);

/**
 * Replaces the defines in the given text.
 *
 * @param text The text to replace defines in.
 * @param lineNum The line number of the text.
 * @return 1 if found an error, 0 if found a define decleration, 2 if found an define use that have been replaced.
 */
int replaceDefines(char *text, int lineNum);

/**
 * Removes comments and spaces from the given line.
 *
 * @param line The line to remove comments and spaces from.
 * @return 1 if a coment, 0 otherwise.
 */
int removeCommentsAndSpaces(char *line);

/**
 * Saves the word in the token.
 *
 * @param token the word to comapre to the saved words.
 * @return 1 if is a saved word, 0 otherwise.
 */
int savedWord(char *token);

#endif
