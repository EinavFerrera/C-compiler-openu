#include "genericH.h"
#include "preAssembler.h"

int main(int argc, char *argv[])
{

	if (argc == 1)
	{
		printf("Error - file name is required!\n");
		return 0;
	}
	/*compiling each file*/
	else
	{
		int i;
		/*read = xxx.as , output - xxx.am*/
		for (i = 1; i < argc; i++)
		{
			if (preAssembel(argv[i]))
			{
				printf("\n\t\t\t************ Pre Assembler finished successfully for file \"%s\"! Am file has been created ************\n", argv[i]);
				return FALSE;
			}
			/*compiling and checks if there were no errors*/
			if (!fAssembler(argv[i]))
			{
				printf("\n\t\t\t************ File \"%s\" compiled successfully! ************ \n\n", argv[i]);
			}
		}
	}
	return FALSE;
}
