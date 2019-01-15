# COEN146_Lab1

main.c:
This program reads in input from a binary file and writes it in a seperate output file. Given a random binary file, the program uses the fread() and fwrite() functions in order to read all the contents on the source file and the write it into the destination file. The files are then closed.

lab1.c:
This program does the same thing as main.c, but copies the content of src.txt into the dest.txt 10 bytes at a time. The code still uses the fread() and fwrite functions to copy the input binary files and write them to the new dest.txt file.

The Objectives:
To setup and demonstrate the use of Unix/ Linux or Mac OS terminal
- To learn and demonstrate the use command – line programs
    -Commands: ls, more, mv, rm, mkdir, rmdir, cd, cp, chmod, who, ps, kill, ctrl+c, cmp, grep, cat, and man
    -Options: -a: all, -b: buffer, -c: command, -d: debug, -e: execute, -f: file, -l: list, -o: output, -u: user
- To learn and demonstrate the use of functions in C
    -Copy one binary file to another, arguments: src file, dest file

Process:
- Declare and open a binary file
    -FILE *fp;
    -fp = fopen(“data.bin”, “rb”); OR fp = fopen(“data.bin”, “wb”);
    -fclose(fp);
Read and write functions for binary files
    -fwrite( ptr, int size, int n, FILE *fp ); àfwrite(&i, sizeof (int), 1, fp);
    -fread( ptr, int size, int n, FILE *fp ); àfwrite(&i, sizeof (int), 1, fp);
