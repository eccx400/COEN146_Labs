 /**************************
	socket example, client
	Winter 2019
 **************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 


int main (int, char *[]);


/********************
 *	main
 ********************/
int main (int argc, char *argv[])
{
	int i;
	int sockfd = 0, n = 0;
	char buff[10];
	char *p;
	struct sockaddr_in serv_addr;
	
	if (argc != 5)
	{
		printf ("Usage: %s <ip of server>  \n",argv[0]);
		return 1;
	} 

	// set up
	memset (buff, '0', sizeof (buff));
	memset (&serv_addr, '0', sizeof (serv_addr)); 

	// open socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf ("Error : Could not create socket \n");
		return 1;
	}

	//Input file to read from src.txt
	FILE * fp = fopen(argv[3], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
	} 

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1])); 

	if (inet_pton (AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
	{
		printf ("inet_pton error occured\n");
		return 1;
	} 
 
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
	{
		printf ("Error : Connect Failed \n");
		return 1;
	} 

	// Destination file authentication
	write(sockfd, argv[4], strlen(argv[4]) + 1);
	read(sockfd, buff, sizeof(buff));

	int total;
	while((total = fread(buff, 1, 10 , fp)) > 0)
	{
		write(sockfd, buff, total);
	}

	/**
	// input, send to server, receive it back, and output it
	while (scanf ("%s", buff) == 1)
	{
		write (sockfd, buff, strlen (buff) + 1);
		read (sockfd, buff, sizeof (buff));
		printf ("%s\n", buff);
	}
	*/

	close (sockfd);

	fclose(fp);
	return 0;

}
