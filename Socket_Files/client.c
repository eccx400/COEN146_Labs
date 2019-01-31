 /**************************
	TCP socket example, client
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

#define	SIZE 10

int main (int, char *[]);


/********************
 * main
 ********************/
int main (int argc, char *argv[])
{
	int		i;
    int		sockfd = 0, n = 0;
    char	buff[SIZE];
	char	*p;
    struct	sockaddr_in serv_addr; 
	int		r;
	FILE	*fp;

    if (argc != 5)
    {
		printf ("Usage: %s <ip of server> src_file dest_file\n",argv[0]);
		return 1;
    } 

	// set up
    memset (buff, '0', sizeof (buff));
    memset (&serv_addr, '0', sizeof (serv_addr)); 

	// open socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
		printf ("Error: Could not create socket \n");
		return 1;
    } 

	// set address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (atoi (argv[1])); 

    if (inet_pton (AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
    {
		printf ("inet_pton error occured\n");
		return 1;
    } 

	// connect
    if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
    {
		printf ("Error: Connect failed \n");
		return 1;
    } 

	if ((fp = fopen (argv[3], "rb")) == NULL)
	{
		printf ("Error: could not opem file\n");
		return 1;
	}

	write (sockfd, argv[4], strlen(argv[3]) + 1);
	read (sockfd, buff, 1);
	
	// input, send to server, receive it back, and output it
	while ((r = fread (buff, sizeof (char), SIZE, fp)) > 0)
		write (sockfd, buff, r);

	fclose (fp);
	close (sockfd);

    return 0;
}
