/**************************
	UDP socket example, client
	Winter 2019
 **************************/

#include "tfv2.h"
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

/**
 * @author Eric Cheng
 * @date 4 February, 2019
 *
 * This project consists of building an S&W (Stop and Wait) reliable protocol. TFv2 is going to be built on top
 * of UDP, and it is supposed to provide a reliable transport service to TFv1 (developed in week 3, which needs
 * to change to call your new send and receive functions and use buffers of same size). Messages are sent one
 * at a time, and each message needs to be acknowledged when received, before a new message can be sent.
 * TFv2 implements basically the protocol rdt2.2 presented in the text book.
 */

/********************
 * main
 ********************/
int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in server_addr;
	struct hostent *host;
	char send_data[10]; // Send 10 bytes at a time
	socklen_t addr_len;
	host= (struct hostent *) gethostbyname((char *)"127.0.0.1");
	int state = 0; // Start in 0 state
	
	
	// Checks to see number of arguments (./client, IP, Port, input file, output file)
	if (argc != 5)
        {
                printf ("Usage: %s <ip of server>  \n",argv[0]);
                return 1;
        }

	PACKET * a = (PACKET * ) malloc(sizeof(PACKET)); //Send
	PACKET * b = (PACKET * ) malloc(sizeof(PACKET));  //Response 

	/**
	a -> HEADER.seq_ack = 0;
	a -> HEADER.length = 0;
	a -> HEADER.checksum = 0;
	for(int i = 0; i < SIZE; i++)
	{
		a.data[i] = 0;
	}
	*/

	// open socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	//Input file to read from src.txt
	FILE * fp = fopen(argv[3], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
	} 

	// set address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000); // Makes port into integer
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);	

	int ack_num, length, count, lol;

	//
	memcpy(a->data, argv[4], 10);
	a->header.seq_ack = state;
	a->header.length = strlen(argv[4]) + 1;

	// Does the checksum
	do
	{
		count = 0;
		(a)->header.checksum = 0;
		(a)->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);
		printf("Checksum Value:%d\n",(a)->header.checksum);
		
		if( count == 3)
		{
			exit(EXIT_FAILURE);
		}
		else
		{	
			count++;		
			// Send to server
			sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);

			//Receive from server
			recvfrom (sock, b, sizeof(PACKET), 0, NULL, NULL);
			lol = b->header.checksum;
			b-> header.checksum = 0;
			b->header.checksum = calc_checksum(b, sizeof(HEADER) + b->header.length);
			continue;
		}	
	}while((b)->header.seq_ack != state && lol != b->header.checksum);

	state = (state + 1) % 2;

	while(!feof(fp)) //While there is still data in the input file
	{
		// Get the input data from the file (Reads by 10 bytes)
		length = fread(send_data, sizeof(char), 10, fp);
		memcpy((*a).data, send_data, sizeof(PACKET));

		// Setup header data for sending packet
		(a)->header.seq_ack = state;
		(a)->header.length = length;

		// Does the checksum
		do
		{
			count = 0;
			(a)->header.checksum = 0;
			(a)->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);
			printf("Checksum Value:%d\n",(a)->header.checksum);
			
			if( count == 3)
			{
				exit(EXIT_FAILURE);
			}
			else
			{	
				count++;		
				// Send to server
				sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);

				//Receive from server
				recvfrom (sock, b, sizeof(PACKET), 0, NULL, NULL);
				lol = b->header.checksum;
				b-> header.checksum = 0;
				b->header.checksum = calc_checksum(b, sizeof(HEADER) + b->header.length);
				continue;
			}	
		}while((b)->header.seq_ack != state && lol != b->header.checksum);

		//Exit loop if return right ack
		state = (state + 1) % 2; // Alternate the states between 0 and 1
	}
	
	a->header.seq_ack = state;
	a->header.length = 0;
	memset(a->data, 0, 10);

	
	// Does the checksum
	do
	{
		count = 0;
		(a)->header.checksum = 0;
		(a)->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);
		printf("Checksum Value:%d\n",(a)->header.checksum);
			
		if( count == 3)
		{
			exit(EXIT_FAILURE);
			printf("Ran 3 times; exit");
		}
		else
		{	
			count++;		
			// Send to server
			sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);

			//Receive from server
			recvfrom (sock, b, sizeof(PACKET), 0, NULL, NULL);
			lol = b->header.checksum;
			b-> header.checksum = 0;
			b->header.checksum = calc_checksum(b, sizeof(HEADER) + b->header.length);
			continue;
		}	
	}while((b)->header.seq_ack != state && lol != b->header.checksum);
	
	close(sock);
	free(a);
	free(b);
	fclose(fp);
	
	/**
	while (1)
	{
   		printf("Client: Type a message (OR q/ Q to quit): \t");
    		//fread(a, 1, )

   		if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)
		{
      			 break;
		}
		else
		{
      			 sendto(sock, send_data, strlen(send_data), 0,
     					 (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); //send to server
		}
   	}
	*/
	return 0;
}

