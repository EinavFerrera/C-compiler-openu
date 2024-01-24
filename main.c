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
		for (i = 1; i < argc; i++)
		{
			preAssemblerMatan(argv[i]); /*read = xxx.as , output - xxx.am*/
			fileCompiling(argv[i]);
			printf("************ Pre Assembler finished successfully for file %s! ************\n", argv[i]);

			/*compiling and checks if there were no errors*/
			if (!fAssembler(argv[i]))
			{
				printf("************ File \"%s\" compiled successfully! ************ \n", argv[i]);
			}
		}
	}
	return 0;
}
