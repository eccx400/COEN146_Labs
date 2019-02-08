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
 */

/*********************
 * main
 *********************/
int main(int argc, char * argv[])
{
    int sock;
    int bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr , client_addr;
    socklen_t addr_len, caddr_len;
    int state;

	PACKET * a = (PACKET * ) malloc(sizeof(PACKET)) ; //Send
	PACKET * b = (PACKET * ) malloc(sizeof(PACKET)); //Response

	//Open socket
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
	// Set address
    	server_addr.sin_family = AF_INET;
   	server_addr.sin_port = htons(5000);
    	server_addr.sin_addr.s_addr = INADDR_ANY;

	 //Bind socket to address
   	 if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    	{
        	perror("Bind");
        	exit(1);
    	}
  	addr_len = sizeof(struct sockaddr);
	printf("\t\t\t\t\t\t\nUDPServer Waiting for client on port 5000\n");
	
	int length, acknum, j;
	char data[10];
	while(length > 0)
	{
		//recvfrom(int sockfd, void *buf, size_t len, int flags, struct(sockaddr *) &src_addr, socklen_t *addrlen);
		bytes_read = recvfrom (sock, a, sizeof(PACKET), 0 , (struct sockaddr *) &client_addr, &addr_len);

		length = (a)->header.length;
		acknum = (a)->header.seq_ack;
		strcpy(data, (*a).data);
		for(j = 0; j < 9; j++)
		{
			printf("Output is:  %s\n", data[j]);
		}
		
		//Checksum
		int check_sum = (a)->header.checksum;
		(a)->header.checksum = 0;
		state = (acknum + 1) % 2;

		if(a->header.checksum != check_sum)
		{	
			(b)->header.seq_ack = state; // Checksum failed
			sendto (sock, b, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
		}

		(b)->header.seq_ack = acknum;
		sendto (sock, b, sizeof(PACKET), 0, (struct sockaddr *)&client_addr, addr_len);
	}
	free(a);
	free(b);
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
