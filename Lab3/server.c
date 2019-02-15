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
 * @author: Eric Cheng
 * @date: 4 February, 2019
 *
 * This project consists of building an S&W (Stop and Wait) reliable protocol. TFv2 is going to be built on top
 * of UDP, and it is supposed to provide a reliable transport service to TFv1 (developed in week 3, which needs
 * to change to call your new send and receive functions and use buffers of same size). Messages are sent one
 * at a time, and each message needs to be acknowledged when received, before a new message can be sent.
 * TFv2 implements basically the protocol rdt2.2 presented in the text book.
 *
 * For testing, start with the server first.
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
	PACKET * b; //Response

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
	int length, acknum, j;
	while(1)
	{
		//recvfrom(int sockfd, void *buf, size_t len, int flags, struct(sockaddr *) &src_addr, socklen_t *addrlen);
		bytes_read = recvfrom (sock, a, sizeof(PACKET), 0 , (struct sockaddr *) &client_addr, &addr_len);
		printf("Packet Data: %s\n", bytes_read);
		perror("Okay");
		printf("Okay!\n");
	
		//Checksum
		int check_sum = (a)->header.checksum;
		(a)->header.checksum = 0;

		length = (a)->header.length;
		acknum = (a)->header.seq_ack;
		memcpy(recv_data, a->data, sizeof(PACKET));
		printf("Packet Data: %s\n", bytes_read);
		
		if(calc_checksum(a, sizeof(HEADER) + a->header.length) != check_sum)
		{
			printf("The checksum failed");	
			(b)->header.seq_ack = (acknum + 1) % 2; // Checksum failed
			sendto (sock, a, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
			continue;
		}
		
		// If output file not created, fopen to open it; if created; fwrite to write to it
		if(np == NULL)
		{
			np = fopen(a->data, "wb");
			printf("The output file has been created with input data");		
		}
		else
		{
			fwrite(a->data, 1, length, np);
			printf("Write into the existing output file");
		}

		(b)->header.seq_ack = acknum;
		sendto (sock, b, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
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
