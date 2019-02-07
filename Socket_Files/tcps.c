/**************************
 *     TCP socket example, server
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
    int sock, connected, bytes_recieved , true = 1;  
    char send_data [1024] , recv_data[1024];       
    struct sockaddr_in server_addr,client_addr;    
    int sin_size;
//Open socket        
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
//Set address
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(5000);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
//Bind socket
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }
    printf("\nTCPServer Waiting for client on port 5000: \t");
//Listen
    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }
    sin_size = sizeof(struct sockaddr_in);
    while(1){
        //Accept
        connected = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
        printf("I got a connection from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        while ((bytes_recieved = recv(connected,recv_data,1024,0))>0){  
            printf("\t\t\t\t Server: The Client (%s , %d) said: %s\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_data);
            write(connected,"",7);
        }
    close(connected);
    }
    close(sock);
    return 0;
} 
