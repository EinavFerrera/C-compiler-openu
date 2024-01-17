#ifndef firstAssembler
#define firstAssembler

void fAssembler(char *fileName);
cNode *searchNode(cNode head, char *target, int srcAttr);
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr);
lNode searchListNode(lNode head, char *target, int srcAttr);
int validateLabelList(lNode labelListDeclared, cNode headNode);
void AREAssign(cNode headNode, lNode labelList);
void numOfLinesAssign(cNode headNode, int *IC, int *DC);
void validOperandPerCode(cNode node);
void fixLineaddress(cNode headNode, int IC);
void addaddressToLabelList(lNode labelListhead, cNode headNode);
void addLabelsAddressToCnode(cNode headNode, lNode labelList);
void freeAll(cNode headNode, lNode labelList);
#endif