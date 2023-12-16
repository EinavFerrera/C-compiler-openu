typedef struct node
{
	char* originalLine;
	int lineNum;		    /*line number in the original file*/
    int isMcr;	            /*is a macro - T\F*/
	int startMcr;		    /*line where the macro begins*/
	int endMcr; 		    /*line where the macro ends*/
	int lableAddressLine;   /*address of label in ob*/
	int operandCount;   	/*number of operands if its a command*/
	int operandType[2];     /*[0] - operand 1. [1] - operand 2*/
	int operandImm[2];	    /*if the operand is number*/
    int operandReg[2]; 	    /*if the operand is register*/
    char *operandLabel[2];  /*if the operand is label*/
	int ARE;			    /*A/R/E address*/
	char labelText[MAX_LINE_SIZE];/*original label*/
    int lineSize;	        /*number of words needed in the IC\DC*/
    int linetType;          /*type of the instruction - DATA/CODE*/
	node next;			/*pointer to the next struct in the linked list*/
} Item;

node createNode(char *name, int address)
{
	gNode newNode = (gNode)calloc(1, sizeof(Item));
	newNode->name = (char *)calloc(1, (strlen(name) + 1) * sizeof(char));
	strcpy(newNode->name, name);
	newNode->startMacro = 0;
	newNode->endMacro = 0;
	newNode->address = address;
	newNode->commandIndex = -1;
	newNode->operandType[0] = NO_ADDRESS;
	newNode->operandType[1] = NO_ADDRESS;
	newNode->operandType[2] = NO_ADDRESS;
	newNode->ARE = 0;
	newNode->type = CODE;
	newNode->next = NULL;
	return newNode;
}