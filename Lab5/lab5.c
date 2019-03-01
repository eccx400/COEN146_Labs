#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

/**
 * Will need 4 machines
 */
typedef struct
{
	char name[50];
	char ip[50]
	int port;
}MACHINE;

/**
 * Global Variables
 */
int matrix[n][n];
Machine linux_machines[n];

/**
 * Thread functions
 */
void * linkState(void *);
void * receiveInfo(void *);

void printTable(void);


int main(int argc, char * argv[])
{
	return 0;
}

void * receiveInfo()
{

}

void * linkState()
{

}

void printTable()
{

}
