typedef struct contentNode *cNode; /*definition of pointer to generic struct*/
typedef struct mcrNode *mNode;

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
    cNode next;                           /*pointer to the next node in list*/
    int lableAddressLine;                 /*address of label in ob*/
    int ARE;                              /*A/R/E address*/
    int lineSize;                         /*number of words needed in the IC\DC*/
    int numOfWords;                       /*number of words in data line*/
    int *dataArray;                       /*pointer to the data array*/
} contentNode_object;

typedef struct mcrNode
{
    char name[MAX_LINE_SIZE]; /*name of the macro*/
    char *mcrContent;         /*pointer to the macro content*/
    struct mcrNode *next;
} mcr_object;

// mNode createNewMcrNode(char *name, char *content, mNode *head);
cNode createNewNode(char *line, int lineAdress, cNode *head);
void nodeInit(cNode *node, cNode *head);
void typeProperties(cNode *node, char *token);
void codeProperties(cNode *node, char *token);
void printNode(cNode node);
void printLoop(cNode head);
void insertDataIntNode(cNode *node, char *token);
void insertDataStringNode(cNode *node, char *token);
