/**************************
	UDP socket example, client
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


/********************
 * main
 ********************/
int main(int argc, char * argv[])
{
int sock;
struct sockaddr_in server_addr;
struct hostent *host;
char send_data[1024];
socklen_t addr_len;
host= (struct hostent *) gethostbyname("127.0.0.1");

// open socket
if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
{
perror("socket");
exit(1);
}

// set address
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(5000);
server_addr.sin_addr = *((struct in_addr *)host->h_addr);

while (1)
{
    printf("Client: Type a message (OR q/ Q to quit): \t");
    scanf("%s", send_data); //input message

    if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0)
       break;

    else
       sendto(sock, send_data, strlen(send_data), 0,
              (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); //send to server
   }

}

