/**************************
 *     UDP socket example, server
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


/*********************
 * main
 *********************/
int main()
{
    int sock;
    int bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr , client_addr;
    socklen_t addr_len;
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
	while (1)
	{
        //Receive from client
        bytes_read = recvfrom(sock,recv_data,1024,0, (struct sockaddr *)&client_addr, &addr_len);
	    recv_data[bytes_read] = '\0';
        printf("\t\t\t\t Server: The Client (%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        printf("%s\n", recv_data);
    }
  return 0;
}
