typedef struct contentNode *cNode; /*definition of pointer to generic struct*/
typedef struct mcrNode *mNode;
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

typedef struct mcrNode
{
    char name[MAX_LINE_SIZE]; /*name of the macro*/
    char *mcrContent;         /*pointer to the macro content*/
    struct mcrNode *next;
} mcr_object;

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

// mNode createNewMcrNode(char *name, char *content, mNode *head);
cNode createNewNode(char *line, int lineaddress, cNode *head);
void nodeInit(cNode *node, cNode *head);
void typeProperties(cNode *node, char *token);
int codeProperties(cNode *node, char *token);
void printNode(cNode node);
void printListLoop(lNode head);
void printLoop(cNode head);
void insertDataIntNode(cNode *node, char *token);
void insertDataStringNode(cNode *node, char *token);
int isOnlyDigit(char *token);
int isOnlyAlpha(char *token);
void addNode(cNode head, cNode node);
void setNextNode(cNode node, cNode nextNode);
int commasCounter(char *line);
void clearDuplicateBlankChars(char *line, char *newLine);
int hasSpacesInWord(char *line);
char *clearHeadAndTailBlanks(char *line);
void clearAllBlanks(char *line);