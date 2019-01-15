#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/**
 * This function determines the length of a given file
 * @param fp = The file which length is to be determined
 */
long long fileSize(FILE * fp)
{
	fseek(fp, 0L, SEEK_END);
	long long size = ftell(fp);
	rewind(fp);
	return size;
}

/**
 * The main function allocates space in an array given
 * the file size and them writes into the file what it
 * has read from the input text file
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

	long long length = fileSize(fp);
	char * arr = (char*)malloc(length + 1);
	
	fread(arr, length, 1, fp);	
	fwrite(arr, length, 1, np);

	fclose(fp);
	fclose(np);
	return 0;
}
