/**************************
 *     UDP socket example, server
 *     Winter 2019
 ***************************/

#include "tfv2.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

/**
 * @author Eric Cheng
 * @date 17 February, 2019
 *
 * This project builds on top of the Lab 3 TFv2 to enable it to deal with loss of messages.
 * TFv3 implements basically the protocol rdt3.0 presented in the textbook. It consists of 
 * a client and server. Communication is unidirectional, i.e. data flows from the client 
 * to server. The server starts and waits for a message, and the client starts the communication.
 * Messages have sequence or ack number 0 or 1 (start with 0). Before sending each message
 * a checksum is calculated and added to the header. After sending each message the client
 * starts a timer. Use select for that. If select returns 0 there is no data; if it returns
 * 1, there is data so the client calls recvfrom to receive ACK and processes it. If ACK
 * is not corrupted and the ack number is right, the client can now send one more message.
 *
 * The server then checks the checksum and then sends an ACK if seq # and checksum is correct.
 * It can then write the data to the output file.
 */

/*********************
 * main
 *********************/
int main(int argc, char * argv[])
{
	int sock;
	int bytes_read;
	char recv_data[10];
	struct sockaddr_in server_addr , client_addr;
	struct sockaddr_storage server_storage;
	socklen_t addr_len, caddr_len;
	int state = 0; // Start in 0 state wait

	if(argc != 2)
	{
		printf ("Usage: %s <port> \n", argv[0]);
		return 1;
	}
	
	PACKET * a = (PACKET * ) malloc(sizeof(PACKET)) ; //Send
	//PACKET * b; //Response

	//Open socket
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Socket");
            exit(1);
        }

	// Set address
    	server_addr.sin_family = AF_INET;
   	server_addr.sin_port = htons((short)atoi(argv[1]));
    	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset ((char *) server_addr.sin_zero, '\0', sizeof (server_addr.sin_zero));

	 //Bind socket to address
   	if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0)
    	{
        	perror("Bind");
        	exit(1);
    	}
  	addr_len = sizeof(server_storage);
	printf("UDP server waiting for packages\n");
	
	FILE * np = NULL;
	int length, acknum, random;
	while(1)
	{
		//recvfrom(int sockfd, void *buf, size_t len, int flags, struct(sockaddr *) &src_addr, socklen_t *addrlen);
		recvfrom (sock, a, sizeof(PACKET), 0 , (struct sockaddr *) &client_addr, &addr_len);
		perror("Okay");
		
		if(a->header.length == 0)
		{
			break;
		}
	
		//Generates error
		random = rand() % 100 + 1;	
		if(random < 10)
		{
			printf("Skips an ACK to fake error and loss effect\n");
			continue;
		}
		
		//Checksum
		int check_sum = (a)->header.checksum;
		(a)->header.checksum = 0;
		(a)->header.checksum = calc_checksum(a, sizeof(HEADER) + a->header.length);

		length = (a)->header.length;
		acknum = (a)->header.seq_ack;
		
		printf("The checksum is: %d\n", check_sum);
		printf("The packet checksum is: %d\n", a->header.checksum);
		if( a->header.checksum != check_sum)
		{
			printf("The checksum failed\n");	
			(a)->header.seq_ack = (acknum + 1) % 2; // Checksum failed
			sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
			continue;
		}
		
		// If output file not created, fopen to open it; if created; fwrite to write to it
		if(np == NULL)
		{
			np = fopen(a->data, "wb");
			printf("The output file has been created with input data\n");		
		}
		else
		{
			fwrite(a->data, 1 , length, np);
			printf("Write into the existing output file: \n");
		}

		//(b)->header.seq_ack = acknum;
		sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
		state = (state + 1) % 2;
	}
	fclose(np);
	//free(a);
	//free(b);
	/**
	while (1) {
   	  	//Receive from client
	        bytes_read = recvfrom(sock,recv_data,1024,0, (struct sockaddr *)&client_addr, &addr_len);
		    recv_data[bytes_read] = '\0';
       		printf("\t\t\t\t Server: The Client (%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
       	 	printf("%s\n", recv_data);
  	}
	*/
 	return 0;
}
