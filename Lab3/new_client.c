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
	int bytes_read;
	struct sockaddr_in server_addr;
	struct hostent *host;
	char send_data[10]; // Send 10 bytes at a time
	socklen_t addr_len;
	//host= (struct hostent *) gethostbyname((char *)"127.0.0.1");
	int state = 0; // Start in 0 state
	
	
	// Checks to see number of arguments (./client, IP, Port, input file, output file)
	if (argc != 5)
        {
                printf ("Usage: %s <ip of server> <port> <input file> <output file> \n",argv[0]);
                return 1;
        }

	PACKET * a = (PACKET * ) malloc(sizeof(PACKET)); //Send
	PACKET * b = (PACKET * ) malloc(sizeof(PACKET));  //Response 

	printf("Pre precheck stuff\n");
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
	printf("Precheck stuff\n"); 

	// set address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1])); // Makes port into integer
	//server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	inet_pton(AF_INET, argv[2], &server_addr.sin_addr.s_addr);	
	memset (server_addr.sin_zero, '\0', sizeof (server_addr.sin_zero));
	addr_len = sizeof(server_addr);	

	sock = socket (PF_INET, SOCK_DGRAM, 0);
		
	int length, count, acknum;
	length = 10;
	count = 0;
	printf("Check stuff\n");

	a->header.checksum = 0;
	memcpy(a->data, argv[4], 10);
	a->header.seq_ack = state;
	a->header.length = strlen(argv[4]) + 1;
	a->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);
	sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);
	printf("Client do yo stuff, Packet created\n");

	while(bytes_read = recvfrom (sock, a, sizeof(PACKET), 0 , (struct sockaddr *) &server_addr, &addr_len) > 0)
	{
		printf("Goes in while loop\n");
		if(b->header.seq_ack != state)
		{
			if(count <= 3)
			{	
				count++;
				sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);
			}
			else
			{
				exit(1);
			}
		}
		else
		{
			count = 0;
			state = (state + 1) % 2; // Alternate state between 0 and 1
			break;
		}
	} 
	printf("Completes first while loop \n");
	
	while(!feof(fp))
	{
		// Create packet
		int bytes_length = fread(send_data, 1, 10, fp);
		(a)->header.checksum = 0;
                int check_sum = calc_checksum(a, sizeof(HEADER) + a->header.length);
		(a)->header.seq_ack = state;
		(a)->header.length = bytes_length;
		memcpy(a->data, send_data ,bytes_length);
                printf("Checksum Value:%d\n",(a)->header.checksum);

		// Add Randomizer
		if(rand() % 100 < 20)
		{
			a->header.checksum = 0;
		}
		a->header.checksum = check_sum;
		sendto(sock, a, sizeof(PACKET), 0, (struct sockaddr *) &server_addr, addr_len);
			
		printf("Complete packet creation and sent\n");
		while(bytes_read = recvfrom (sock, a, sizeof(PACKET), 0, (struct sockaddr *) &server_addr, &addr_len) > 0)
		{	
			if(b->header.seq_ack != state)
               		{
                        	if(count <= 3)
                        	{
                                	count++;
					printf(a->data);
                                	sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);
                       		}
                        	else
                       		{
                                	exit(1);
                        	}
                	}
                	else
                	{
				count = 0;
                       		state = (state + 1) % 2; // Alternate state between 0 and 1
				break;
                	}
		}
	}
	printf("Finishes second loop\n");
		
	(a)->header.checksum = 0;
	(a)->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);
	(a)->header.seq_ack = state;
	(a)->header.length = 0;
	memcpy(a->data, '\0', sizeof(a->data));
	sendto(sock, a, sizeof(PACKET), 0, (struct sockaddr *)&server_addr, addr_len);

	close(sock);
	//free(a);
	//free(b);
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

