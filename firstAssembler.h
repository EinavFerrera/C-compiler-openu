#ifndef firstAssembler
#define firstAssembler

void fAssembler(char *fileName);
cNode *searchNode(cNode head, char *target, int srcAttr);
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr);
lNode searchListNode(lNode head, char *target, int srcAttr);
int validateLabelList(lNode labelListDeclared, cNode headNode);

#endif