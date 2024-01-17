int sAssembler(char *fileName, cNode headNode, lNode labelList, int codeLength, int IC, int DC);
int nodesToBinary(cNode headNode, lNode labelList, char *binaryCode);
void addStrToBinaryCode(char *tempStr, int value, int size);
void binaryToSigns(FILE *outputFile, char *binaryCode, int IC, int DC);
void createEntryExternFile(lNode labelList, int searchAttr, char *fileName);
