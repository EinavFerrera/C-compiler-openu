#ifndef firstAssembler
#define firstAssembler

/**
 * first Assembler pass on the given file.
 *
 * @param fileName The name of the file to assemble.
 * @return Returns an integer indicating the success or failure of the assembly process. 1 for failure, 0 for success.
 */
int fAssembler(char *fileName);

/**
 * removes the /r that is added on UBUNTU
 *
 * @param line The line to remove the /r from.
 */
void removeCarrigeReturn(char *line)

    /**
     * Searches for a node in the given list with the specified target and source attribute.
     *
     * @param head The head node of the list.
     * @param target The target to search for as a string.
     * @param srcAttr The source attribute to match as an integer.
     * @return Returns a pointer to the found node, or NULL if not found.
     */
    cNode searchNode(cNode head, char *target, int srcAttr);

/**
 * Creates a new labels list based on the original list and the given attributes.
 *
 * @param orgListHead The head node of the original list.
 * @param newListHead The head node of the new list.
 * @param listAttr An array of attributes for the new list.
 * @param numOfAttr The number of attributes in the array.
 * @return Returns the head node of the new labels list.
 */
lNode createLabelsList(cNode orgListHead, lNode newListHead, int *listAttr, int numOfAttr);

/**
 * Searches for a node in the given labels list with the specified target and source attribute.
 *
 * @param head The head node of the labels list.
 * @param target The target to search for.
 * @param srcAttr The source attribute to match.
 * @return Returns a pointer to the found node, or NULL if not found.
 */
lNode searchListNode(lNode head, char *target, int srcAttr);

/**
 * Validates the label list against the given head node.
 *
 * @param labelListDeclared The declared label list.
 * @param headNode The head node of the code list.
 * @return Returns an integer indicating the success or failure of the validation process.
 */
int validateLabelList(lNode labelListDeclared, cNode headNode);

/**
 * Assigns the ARE (Absolute/Relative/External) values to the given head node based on the label list.
 *
 * @param headNode The head node of the code list.
 * @param labelList The label list.
 */
void AREAssign(cNode headNode, lNode labelList);

/**
 * Assigns the number of lines (IC and DC) to the given head node.
 *
 * @param headNode The head node of the code list.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 */
void numOfLinesAssign(cNode headNode, int *IC, int *DC);

/**
 * Validates the operand per code in the given node.
 *
 * @param node The node to validate.
 */
void validOperandPerCode(cNode node);

/**
 * Fixes the line address in the given head node based on the instruction counter.
 *
 * @param headNode The head node of the code list.
 * @param IC The instruction counter.
 */
void fixLineaddress(cNode headNode, int IC);

/**
 * Adds the address to the label list.
 *
 * @param labelListhead The head node of the label list.
 * @param headNode The head node of the code list.
 */
void addAddressToLabelList(lNode labelListhead, cNode headNode);

/**
 * Adds the labels addresses to the code nodes.
 *
 * @param headNode The head node of the code list.
 * @param labelList The label list.
 */
void addLabelsAddressToCnode(cNode headNode, lNode labelList);

/**
 * Frees the memory allocated for the code list and label list.
 *
 * @param headNode The head node of the code list.
 * @param labelList The head node of the label list.
 */
void freeAll(cNode headNode, lNode labelList);

#endif
