#ifndef secondAssembler
#define secondAssembler

/**
 * Assembles the given file into binary code.
 *
 * @param fileName   The name of the file to assemble.
 * @param headNode   The head node of the code nodes.
 * @param labelList  The head node of the label nodes.
 * @param codeLength The length of the code.
 * @param IC         The instruction counter.
 * @param DC         The data counter.
 * @return           Returns 0 on success, -1 on failure.
 */
int sAssembler(char *fileName, cNode headNode, lNode labelList, int codeLength, int IC, int DC);

/**
 * Converts the code nodes and label nodes into binary code.
 *
 * @param headNode    The head node of the code nodes.
 * @param labelList   The head node of the label nodes.
 * @param binaryCode  The binary code to store the converted code.
 * @return            Returns 0 on success, -1 on failure.
 */
int nodesToBinary(cNode headNode, lNode labelList, char *binaryCode);

/**
 * Adds a string representation of a value to the binary code.
 *
 * @param tempStr  The temporary string to store the binary representation.
 * @param value    The value to convert to binary.
 * @param size     The size of the binary representation.
 */
void addStrToBinaryCode(char *tempStr, int value, int size);

/**
 * Converts the binary code into signs and writes them to the output file.
 *
 * @param outputFile   The output file to write the signs to.
 * @param binaryCode   The binary code to convert.
 * @param IC           The instruction counter.
 * @param DC           The data counter.
 */
void binaryToSigns(FILE *outputFile, char *binaryCode, int IC, int DC);

/**
 * Creates an entry/extern file based on the label list and search attribute.
 *
 * @param labelList     The head node of the label nodes.
 * @param searchAttr    The search attribute (1 for entry, 2 for extern).
 * @param fileName      The name of the file to create.
 */
void createEntryExternFile(lNode labelList, int searchAttr, char *fileName);

#endif
