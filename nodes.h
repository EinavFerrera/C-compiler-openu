typedef struct contentNode *cNode; /*definition of pointer to generic struct*/
typedef struct listNode *lNode;

typedef struct contentNode
{
    char originalLine[MAX_LINE_SIZE];     /*original line from the file*/
    int lineNum;                          /*line number in the original file*/
    int lineType;                         /*type of the instruction - DATA_INT/DATA_STRING/CODE/ENTRY/EXT*/
    char labelText[MAX_LABEL_SIZE];       /*original label*/
    int opCode;                           /*the codenum of the operation*/
    int operandCount;                     /*number of operands in the line for CODE*/
    int operandType[2];                   /*[0] - operand 1. [1] - operand 2|| imm,reg,label,dataLabel*/
    char operandLabel[2][MAX_LABEL_SIZE]; /*if the operand is label*/
    int operandImm[2];                    /*if the operand is number*/
    int operandReg[2];                    /*if the operand is register*/
    int targetLabelAdd[2];                /*if the operand is DATA LABEL or LABEL - the address where this label is*/
    int lableAddressLine;                 /*address of label in ob*/
    int ARE;                              /*A/R/E address*/
    int lineSize;                         /*number of words needed in the IC\DC*/
    int numOfWords;                       /*number of words in data line*/
    int errorCount;                       /*count of errors in the line*/
    int commaErrorFlag;                   /*flag for comma error*/
    int *dataArray;                       /*pointer to the start of array of data*/
    cNode next;                           /*pointer to the next node in list*/
} contentNode_object;

typedef struct listNode
{
    char name[MAX_LABEL_SIZE]; /*name of the macro*/
    int lineNumber;            /*pointer to the macro content*/
    int mainType;              /*type of DECLERATION or USED LABEL*/
    int type;                  /*ENTRY/EXT/DATA_LABAL/LABEL/OP_LABEL*/
    int address;               /*final address of the label*/
    int EXTUsedAddress;        /*the address where EXT is used*/
    int declarationError;      /*for every ENTRY/DATA_LABEL/OP_LABEL => LABEL/EXT. negative for error*/
    lNode next;
} list_object;

/**
 * Creates a new node with the given line and line address.
 *
 * @param line The original line from the file.
 * @param lineaddress The line number in the original file.
 * @param head Pointer to the head of the list.
 * @return The newly created node.
 */
cNode createNewNode(char *line, int lineaddress, cNode *head);

/**
 * Initializes the given node and sets its next node to the head of the list.
 *
 * @param node Pointer to the node to be initialized.
 * @param head Pointer to the head of the list.
 */
void nodeInit(cNode *node, cNode *head);

/**
 * Sets the type properties of the given node based on the given token.
 *
 * @param node Pointer to the node.
 * @param token The token representing the type of the instruction.
 */
void typeProperties(cNode *node, char *token);

/**
 * Sets the code properties of the given node based on the given token.
 *
 * @param node Pointer to the node.
 * @param token The token representing the operation code.
 * @return The number of operands in the line for CODE.
 */
int codeProperties(cNode *node, char *token);

/**
 * Prints the details of the given node.
 *
 * @param node The node to be printed.
 */
void printNode(cNode node);

/**
 * Prints the details of all nodes in the list starting from the given head.
 *
 * @param head The head of the list.
 */
void printListLoop(lNode head);

/**
 * Prints the details of all nodes in the list starting from the given head.
 *
 * @param head The head of the list.
 */
void printLoop(cNode head);

/**
 * Inserts a data integer node with the given token into the given node.
 *
 * @param node Pointer to the node.
 * @param token The token representing the data integer.
 */
void insertDataIntNode(cNode *node, char *token);

/**
 * Inserts a data string node with the given token into the given node.
 *
 * @param node Pointer to the node.
 * @param token The token representing the data string.
 */
void insertDataStringNode(cNode *node, char *token);

/**
 * Checks if the given token contains only digits.
 *
 * @param token The token to be checked.
 * @return 1 if the token contains only digits, 0 otherwise.
 */
int isOnlyDigit(char *token);

/**
 * Checks if the given token contains only alphabetic characters.
 *
 * @param token The token to be checked.
 * @return 1 if the token contains only alphabetic characters, 0 otherwise.
 */
int isOnlyAlpha(char *token);

/**
 * Sets the next node of the given node to the given next node.
 *
 * @param node The node.
 * @param nextNode The next node.
 */
void setNextNode(cNode node, cNode nextNode);

/**
 * Counts the number of commas in the given line.
 *
 * @param line The line to be checked.
 * @return The number of commas in the line.
 */
int commasCounter(char *line);

/**
 * Clears duplicate blank characters in the given line and stores the result in newLine.
 *
 * @param line The line to be cleared.
 * @param newLine The cleared line without duplicate blank characters.
 */
void clearDuplicateBlankChars(char *line, char *newLine);

/**
 * Checks if the given line has spaces within a word.
 *
 * @param line The line to be checked.
 * @return 1 if the line has spaces within a word, 0 otherwise.
 */
int hasSpacesInWord(char *line);

/**
 * Clears all blank characters in the given line.
 *
 * @param line The line to be cleared.
 */
void clearAllBlanks(char *line);
