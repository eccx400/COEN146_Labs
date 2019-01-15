#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/**
 *
 * @author Eric Cheng
 * @date 11 January, 2019
 *
 * This program uses the fread and fwrite file to
 * copy contents from an input file to an output
 * file. This program copies the content 10 bytes
 * at a time.
 *
 * @param argc = The argument count
 * @param argv = The argument vector
 */
int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		printf("Incorrect amount of arguments");
		return -1;
	}

	FILE * fp = fopen(argv[1], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
		return 0;
	}
	FILE * np = fopen(argv[2], "wb");
	if(!np)
	{
		printf("File cannot be opened");
		return 0;
	}

	char * arr[10];
	int length = 10;
	
	while(!feof(fp))
	{	
			int input = fread(arr, 1, length, fp);	
			fwrite(arr, 1, input, np);
	}

	fclose(fp);
	fclose(np);
	return 0;
}
