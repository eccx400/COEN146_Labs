 /**************************	
 *     socket example, server
 *     Winter 2019
 ***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

/**
 *@author Eric Cheng
 *@date 18 January, 2019
 *@class COEN 146 Computer Networks
 *
 *This program models the server side of the client server 
 *model of connection using the TCP form of package transmission.
 *For this program, there will be a binary file being read 
 *and written by ten byte chunks. The data will be read on the
 *client side, and will be sent to the server side to be written
 *in the destination file. The IP address and the port will be
 *specified by the user.
 */
int main (int, char *[]); 

/*********************
 * main
 *********************/
int main (int argc, char *argv[])
{
	int n;
	char *p; 
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char buff[10];

	// Write to output file dest.txt
	FILE * np = NULL;

	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1])); 

	// create socket, bind, and listen
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)); 
	listen (listenfd, 10); 
	
	// accept and interact
	while (1)
	{
		connfd = accept (listenfd, (struct sockaddr*)NULL, NULL); 

		// receive data and reply
		while ((n = read (connfd, buff, sizeof (buff))) > 0)
		{
			/**
			// change lower to upper case 
			p = buff;
			while (*p != '\0')
			{
				if (islower (*p))
					*p = toupper (*p);
				p++;
			}

        		write (connfd, buff, p - buff + 1); 
			*/
			if(np == NULL)
			{
				// Opens the file
				np = fopen(buff, "wb");
				memset(buff, '0', sizeof(buff));
				p = buff;
				*p = '1';
				write(connfd, buff, sizeof(buff));
			}	
			else
			{	
				//Write to the destination file
				fwrite(buff, 1, n, np);
			}	
		}
        	close (connfd);
		fclose(np);
		np = NULL; // Need to set null to return to initialized
	}
	return 0;
}
