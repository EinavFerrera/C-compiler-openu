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
			fileCompiling(argv[i]);
			printf("************ Pre Assembler finished succssesfully for file %s! ************\n", argv[i]);
			preAssemblerMatan(argv[i]); // read = matan.as , output - matan.am
			fAssembler(argv[i]);
			// checks if there where no errors
			// if (errorsNodesCheck == 0)
			printf("************ File \"%s\" compiled successfully! ************ \n", argv[i]);
		}
	}
	return 0;
}
