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
int id;
int port;
pthread my_mutex lock; // Global mutex
Machine linux_machines[n]; // For host tables

/**
 * Thread functions
 */
void * linkState(void *);
void * receiveInfo(void *);

void parseFiles(FILE * fp, FILE * np);
void printTable(void);

// The main function to take in keyboard input and use UDP
int main(int argc, char * argv[])
{
	if (argc != 5)
        {
                printf ("Usage: %s <id> <nmachines> <cost_file> <host_ files> \n",argv[0]);
                return 1;
        }

	FILE * fp = fopen(argv[3], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
	}
	printf("Input file created\n"); 

	FILE * np = fopen(argv[4], "wb");
	if(!np)
	{
		printf("File cannot be opened");
	}
	printf("Output file created\n"); 

	pthread_mutex_init(&lock, NULL);

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

void minDist(int dist, bool sptSet[])
{
	int min = 9999, min_Index;
	int i;
	for(i = 0; i < n; i++)	
	{
		if(sptSet[i] == false && dist[i] <= min)
		{
			min = dist[i];
			min_Index = i;
		}
	}
	return min_Index;
}

void linkState(int cost[][], int router)
{
	int dist[n];
	bool sptSet;

	int i;
	for(i = 0; i < n; i++)
	{
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}

	dist[router] = 0;

	int count;
	for( count = 0; count < n - 1; count++)
	{
		int a = minDist(dist, sptSet);
		sptSet[a] = true;
		
		int v;
		for(v = 0; v < n; v++)
		{
			if(!sptSet[v] && graph[a][v] && dist[a] != INT_MAX && dist[a] + cost[a][v] < dist[v])
			{
				dist[v] = dist[a] + cost[a][v];
			}	
		}
	}
	int x;
	for(x = 0; x < n; x++)
	{
		printf("%d ", dist[x]);
	}
}

// Parses the files using the fscanf() function
void parseFiles(FILE * input, FILE * output)
{
	for(int i = 0; i < n; i++)
	{
		if((fscanf(f_hosts, "%s %s %d", &(hosts[i].name), &(hosts[i].ip), (hosts[i].port))) < 1)
		{
			break;
		}
		print
	}
}

// Receives cost update from other machines
void * receiveInfo()
{
	while(1)
	{
		receive_data(port);
	}
}

// Runs link state algorithm; Use dijkstras algorithm to find shortest path
void * linkState()
{
	int val;
}

// Prints the table
void printTable()
{
	pthread_lock(&my_mutex); // do stuff with cost matrix
	pthread_unlock(&my_mutex); // My mutex should be global and shared throughout the program
}
