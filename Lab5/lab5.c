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
	char ip[50];
	int port;
}MACHINE;

/**
 * Global Variables
 */
int n = 4; // Size of row and column of matrix
int matrix[4][4];
int sock;
int _id;
int _port;
int recvData[3] = {0};
int sendData[3] = {0};
pthread_mutex_t lock; // Global mutex
MACHINE linux_machines[4]; // For host tables

/**
 * Thread functions
 */
void * linkState();
void * sendInfo();
void * receiveInfo();

int minDist(int dist[], int sptSet[]);
void link_State(int cost[n][n], int router);
void printTable(void);

// The main function to take in keyboard input and use UDP
int main(int argc, char * argv[])
{
	perror("Everything has been initialized correctly\n");
	if (argc != 5)
        {
                printf ("Usage: %s <id> <nmachines> <cost_file> <host_files> \n",argv[0]);
                return 1;
        }
	_id = atoi(argv[1]);
	_port = linux_machines[_id].port;

	FILE * fp = fopen(argv[3], "rb");
	if(!fp)
	{
		printf("File cannot be opened");
	}
	printf("Input file created\n"); 

	FILE * np = fopen(argv[4], "rb");
	if(!np)
	{
		printf("File cannot be opened");
	}
	printf("Output file created\n"); 
	
	
	// Parses the files using the fscanf() function
	int i, j;

	//Parsing for input and output files
	for (i = 0; i < n; i++)
	{
		fscanf(np , "%s %s %d", (linux_machines[i].name), (linux_machines[i].ip), &(linux_machines[i].port));
		//printf("%s %s %d", (linux_machines[i].name), (linux_machines[i].ip), (linux_machines[i].port));	
		for(j = 0; j < n; j++)
		{
			fscanf(fp, "%d", &matrix[i][j]);
			//printf("The output is: %d\n", &matrix[i][j]);
		}
		printf("\n");
	}
	perror("Files parsed\n");

	pthread_mutex_init(&lock, NULL);
	pthread_t thread1, thread2;

	struct sockaddr_in server_addr;
	struct sockaddr_storage udp_storage;
	socklen_t addr_len, caddr_len;

	// set address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port); // Makes port into integer
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY); // Get IP Address
	//inet_pton(AF_INET, argv[2], &server_addr.sin_addr.s_addr);	
	memset (server_addr.sin_zero, '\0', sizeof (server_addr.sin_zero));
	addr_len = sizeof(udp_storage);	

	//Threads
	pthread_create(&thread1, NULL, receiveInfo, NULL);
	pthread_create(&thread2, NULL, linkState, NULL);

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

	perror("Thread creation preconditions checked\n");

	int count = 0;
	while( count < 3)
	{
		sleep(10);
		
		int ncost, neighbor;
		struct sockaddr_in destination_addr;

		printf("Input updates using <neighbor> <ncost>:\n", _id);
		scanf("%d %d", &neighbor, &ncost);
		
		pthread_mutex_lock(&lock);
		matrix[_id][neighbor] = ncost;
		matrix[neighbor][_id] = ncost;
		pthread_mutex_unlock(&lock);

		sendData[0] = _id;
		sendData[1] = neighbor;
		sendData[2] = ncost;

		printf("The new distance is: %d\n", ncost);			

		int j;
		for( j = 0; j < n; j++)
		{	
			destination_addr.sin_family = AF_INET;
			destination_addr.sin_port = htons(linux_machines[i].port);
			inet_pton(AF_INET, linux_machines[i].ip, &destination_addr.sin_addr.s_addr);
			memset(destination_addr.sin_zero, '\0', sizeof (destination_addr.sin_zero));	
			addr_len = sizeof(destination_addr);
			
			// open socket
			if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			{
				perror("socket");
				exit(1);
			}	
	
			int x;
			for(x = 0; x < n; x++)
			{
				if(x != _id)
				{
					sendto(sock, &sendData, sizeof(sendData), 0, (struct sockaddr *) &destination_addr, addr_len);
				}
			}
		}
		count++;
	}	
	sleep(30);
	fclose(fp);
	fclose(np);
	return 0;
}

// Finds minimum Distance used for link state
int minDist(int dist[], int sptSet[])
{
	int min = 9999, min_Index;
	int i;
	for(i = 0; i < n; i++)	
	{
		if(sptSet[i] == 0 && dist[i] <= min)
		{
			min = dist[i];
			min_Index = i;
		}
	}
	return min_Index;
}

// Does Dijkstra's algorithm
void link_State(int cost[n][n], int router)
{
	int dist[n];
	int sptSet[n];

	int i;
	for(i = 0; i < n; i++)
	{
		dist[i] = 9999;
		sptSet[i] = 0;
	}

	dist[router] = 0;

	int count;
	for( count = 0; count < n - 1; count++)
	{
		int a = minDist(dist, sptSet);
		sptSet[a] = 1;
		
		int v;
		for(v = 0; v < n; v++)
		{
			if(!sptSet[v] && cost[a][v] && dist[a] != 9999 && dist[a] + cost[a][v] < dist[v])
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

// Receives cost update from other machines
void * receiveInfo()
{
	while(1)
	{
		int nBytes = recvfrom (sock, recvData, sizeof(recvData), 0, NULL, NULL);
		perror("Got information\n");
		printf("Data Received: %d\n", nBytes);

		int data1 = ntohl(recvData[0]);
		int data2 = ntohl(recvData[1]);

		pthread_mutex_lock(&lock);
		matrix[data1][data2] = ntohl(recvData[2]);
		matrix[data2][data1] = ntohl(recvData[2]);
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

// Runs link state algorithm; Use dijkstras algorithm to find shortest path
void * linkState()
{
	int delay;
	while(1)
	{
		delay = (rand() % 10) + 10;
		printf("Sleeptime is: %d\n", delay);
		sleep(delay);

		//printTable();
		
		pthread_mutex_lock(&lock);
		int srcnode;
		for(srcnode = 0; srcnode < n; ++srcnode)
		{
			printf("Node %d\n", srcnode);
			link_State(matrix, srcnode);
		}
		pthread_mutex_unlock(&lock);
		printf("\n");
	}
	return NULL;
}

// Prints the table
void printTable()
{
	//pthread_lock(&my_mutex); // do stuff with cost matrix
	//pthread_unlock(&my_mutex); // My mutex should be global and shared throughout the program
	
	pthread_mutex_lock(&lock);
	int i;
	for(i = 0; i < n; i++)
	{
		int j;
		for(j = 0; j < n; j++)
		{
			printf(" %d", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	pthread_mutex_unlock(&lock);
}
