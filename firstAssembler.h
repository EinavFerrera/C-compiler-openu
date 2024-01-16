#ifndef firstAssembler
#define firstAssembler

void fAssembler(char *fileName);
cNode *searchNode(cNode head, char *target, int srcAttr);
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr);
lNode searchListNode(lNode head, char *target, int srcAttr);
int validateLabelList(lNode labelListDeclared, cNode headNode);
void createEntryExternFile(lNode labelList, int searchAttr);
void AREAssign(cNode headNode, lNode labelList);
void numOfLinesAssign(cNode headNode, int *IC, int *DC);
void validOperandPerCode(cNode node);
void fixLineAdress(cNode headNode, int IC);
void addAdressToLabelList(lNode labelListhead, cNode headNode);
int sAssembler(fileName, headNode, labelList);

#endif