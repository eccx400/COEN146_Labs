#include <stdlib.h>
#include <stdio.h>

#define SIZE 10

typedef struct HEADER {
	int seq_ack; // will always be 0 or 1
	int length; // how many bytes of data you have
	int checksum; // checksum calculated by XORing bytes in packet
};

typedef struct PACKET{
	HEADER header;
	char data[SIZE];
};

int calc_checksum (PACKET * pkt, int nBytes){
	int i = 0;
	char cs = 0;
	char * p = (char *) pkt;
	for(i = 0; i < nBytes; ++i)
	{
		cs ^= *p;
		++p;
	}
	return (int) cs;
}
