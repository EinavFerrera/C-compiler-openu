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
		}
	}
	return 0;
}
