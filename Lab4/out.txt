/**
 * Derrick Lee <dlee3@scu.edu>
 * File Transfer v3 Client
 * 2.15F
 */

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "tfv2.h"

int main (int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    if (argc < 5) {
        // provide verify=1 option to randomly send 0 for checksum
        printf("Usage: %s <port> <ip of server> <input> <output> [verify] \n", argv[0]);
        return 1;
    }

    // configure address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(atoi(argv[1]));
    inet_pton(AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof serverAddr;

    /*Create UDP socket*/
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error\n");
        return 1;
    }

    // read file
    FILE* fp = fopen(argv[3], "rb");
    
    if (!fp) {
        printf("Failed to open input file\n");
        return 1;
    }
    // make sure output file name <= 10 bytes
    if (strlen(argv[4]) > 10) {
        printf("Output filename length should be <= 10\n");
        return 1;
    }

	size_t file_bytes = 0;  // total bytes read 
    int seq_no = 0;

    PACKET* pkt = malloc(sizeof(pkt)); // sending packet
    PACKET* resp = malloc(sizeof(resp)); // response packet
    int length = 10;
    int tries  = 0; // attempts for empty packet 

    int is_sending_filename = 1;

    // timer values
    struct timeval tv;

    int time_resp;
    int retries = 0; // # of retries on no ack

    fd_set readfds;
    fcntl(sock, F_SETFL, O_NONBLOCK);

    // read file, create & send packets
    while (length > 0) {
        // create packet

        // filename
        if (is_sending_filename) {
            pkt = create_packet_str(argv[4], strlen(argv[4]), seq_no);
            is_sending_filename = 0;

        } else {
            pkt = create_packet(fp, seq_no);
            length = pkt->header.length;
            file_bytes += length;
        }

        do {
            // rand test 
            if (argc == 6) {
                // 5 % chance of sending a 0 checksum
                if (rand_range(0, 100) < 5) {
                    pkt->header.checksum = 0;
                } else {
                    pkt->header.checksum = calc_checksum(pkt, sizeof(HEADER) + pkt->header.length);
                }
            }

            FD_ZERO(&readfds);
            FD_SET(sock, &readfds);
            tv.tv_sec = 1;
            tv.tv_usec = 0;

            // send data
            sendto(sock, pkt, sizeof(*pkt), 0, (struct sockaddr*) &serverAddr, addr_size);

            time_resp = select(sock + 1, &readfds, NULL, NULL, &tv);

            // no data, resend
            if (time_resp == 0) {
                if (retries >= 3) {
                    printf("Reached max resends (3) exiting...\n");
                    return 1;
                }

                printf("Timed out, retrying... (try #%d)\n", retries + 1);
                retries += 1;

                continue;
            }

            // reset retries count
            retries = 0;

            // wait for response
            recvfrom(sock, resp, sizeof(*resp), 0, NULL, NULL);

            // invalid ack
            if (resp->header.seq_ack != seq_no) {
                printf("[WARN] Mismatching seq/ack, %d vs %d\n",
                    resp->header.seq_ack,
                    seq_no);
            }

            if (resp->header.seq_ack != seq_no
                && pkt->header.length == 0) {
                tries += 1;
            }

            // only try resending null packets 3 times
            if (resp->header.seq_ack != seq_no
                && pkt->header.length == 0
                && tries >= 3) {
                break;
            }
        } while (resp->header.seq_ack != seq_no); // if ack incorrect, resend

        // successful ack, state 0, ack = 0 or state 1, ack 1

        // next state
        seq_no = (seq_no + 1) % 2;
    }

    printf("Sent %zd byte file\n", file_bytes);

    // clean up
    fclose(fp);

    return 0;
}

