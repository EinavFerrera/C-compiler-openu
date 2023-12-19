typedef struct contentNode
{

    char originalLine[MAX_LINE_SIZE];     /*original line from the file*/
    int lineNum;                          /*line number in the original file*/
    int lineType;                         /*type of the instruction - DATA/CODE/ENTRY/EXT*/
    char labelText[MAX_LABEL_SIZE];       /*original label*/
    char operandLabel[2][MAX_LABEL_SIZE]; /*if the operand is label*/
    int opCode;                           /*the codenum of the operation*/
    int operandImm[2];                    /*if the operand is number*/
    int operandCount;                     /*number of operands in the line for CODE*/
    int operandType[2];                   /*[0] - operand 1. [1] - operand 2|| imm,reg,label,dataLabel*/
    int operandReg[2];                    /*if the operand is register*/
    struct contentNode *next;             /*pointer to the next node in list*/
    int lableAddressLine;                 /*address of label in ob*/
    int ARE;                              /*A/R/E address*/
    int lineSize;                         /*number of words needed in the IC\DC*/
    int numOfWords;                       /*number of words in data line*/
} *contentNode;

typedef struct mcrNode
{
    char name[MAX_LINE_SIZE]; /*name of the macro*/
    char *mcrContent;         /*pointer to the macro content*/
    struct mcrNode *next;
} *mcrNode;

mcrNode createNewMcrNode(char *name, char *content, mcrNode *head);
contentNode createNewNode(char *line, int lineAdress);
void nodeInit(contentNode *node, contentNode *head);
void typeProperties(contentNode *node, char *token);
void codeProperties(contentNode *node, char *token);
void insertNode(contentNode *head, contentNode *tail, contentNode newNode);
void setNext(contentNode list, contentNode toSet);
contentNode *getNext(contentNode list);
void printNode(contentNode node);