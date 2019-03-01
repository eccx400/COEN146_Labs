/**
 * @author Eric Cheng
 * @date 1 March, 2019
 *
 * This project consists of building the code to run the link state
 * algorithm in a router. Assume the topology has N routers.
 * 
 * The following information will be available:
 * - Router ID, which is its index into the tables below, is given
 * at the command line.
 * - Number of nodes, N, in the topology will be given by the command
 * line.
 * - Table with costs, NxN will be obtained from file1 (name given at
 * the command line).
 * - Table with machines, names, IP addresses, and port numbers, Nx3, 
 * will be obtained from file 2 (name given at the command line).
 * 
 * Your main data will be:
 * - Neighbor cost table - contains the cost from every node to every
 * node, initially obtained from file1.
 * - Least cost array - obtained with the link state algorithm from 
 * doing Dijkstra's.
 * 
 * The code will have 3 threads: 
 * - Thread 1 loops forever. It receives messages from other nodes and
 * updates the neighbor cost table. When receiving a new cost c from x
 * neighbor y, it should update both costs: x to y and y to x.
 * - Thread 2 reads a new change from the keyboard every 10 seconds, 
 * updates the neighbor cost table, and sends messages to the other nodes
 * using UDP. It finishes 30 seconds after executing 2 changes. You may 
 * execute this part in the main thread.
 * - Thread 3 loops forever. It sleeps for a random number of seconds (10-20),
 * run the algorithm to update the least costs. After the algorithm executes
 * it outputs the current least costs.
 * 
 * You will need a mutex lock to synchronize the access to the neighbor
 * cost table.
 */
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
#include <sys/select.h>
#include <sys/time.h>
#include <limits.h>

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
int n = 4; // Size of row and column of matrix
int matrix[n][n];
int sock;
pthread my_mutex lock;
Machine linux_machines[n]; // For host tables

/**
 * Thread functions
 */
void * linkState(void *);
void * receiveInfo(void *);

void printTable(void);


int main(int argc, char * argv[])
{
	if (argc != 5)
        {
                printf ("Usage: %s <port> <ip address> <input file> <output file> \n",argv[0]);
                return 1;
        }

	FILE * fp = fopen(argv[3], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
	}
	printf("Input file created\n"); 

	FILE * np = fopen(argv[4], "2b");
	if(!np)
	{
		printf("File cannot be opened");
	}
	printf("Output file created\n"); 

	struct sockaddr_in server_addr , client_addr;
	struct sockaddr_storage udp_storage;
	socklen_t addr_len, caddr_len;

	// set address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1])); // Makes port into integer
	//server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	inet_pton(AF_INET, argv[2], &server_addr.sin_addr.s_addr);	
	memset (server_addr.sin_zero, '\0', sizeof (server_addr.sin_zero));
	addr_len = sizeof(udp_storage);	

	// open socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	//Bind socket to address
   	if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0)
    	{
        	perror("Bind");
        	exit(1);
    	}

	return 0;
}

// Receives cost update from other machines
void * receiveInfo()
{

}

// Runs link state algorithm
void * linkState()
{

}

// Prints the table
void printTable()
{
	pthread_lock(&my_mutex); // do stuff with cost matrix
	pthread_unlock(&my_mutex); // My mutex should be global and shared throughout the program
}
