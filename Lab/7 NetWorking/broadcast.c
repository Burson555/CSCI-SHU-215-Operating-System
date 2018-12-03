/**** test-broadcast.c ****/

/* The parent process broadcasts on BC_PORT
	Le child waits for a message
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>


#define BROADCAST_IP "255.255.255.255"
#define BC_PORT 9999

int sock_bc;

void prep_sockets() {

	struct sockaddr_in servad;
	int on = 1;

	if ((sock_bc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("socket creation");
		exit(1);
	}
	
	setsockopt(sock_bc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	setsockopt(sock_bc, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	/* naming */
	memset((void *)&servad,0, sizeof(servad));
	servad.sin_addr.s_addr = htonl(INADDR_ANY);
	servad.sin_family = AF_INET;
	servad.sin_port = htons(BC_PORT);
	if (bind(sock_bc,(struct sockaddr *)&servad,sizeof(servad)) < 0) {
		perror("bind");
		exit(2);
	}
	
}


int main(int argc, char *argv[]) {

	struct sockaddr_in dest, exp;
	int fromlen = sizeof(exp);
	int round_nb = 0;
	
	prep_sockets();	

	if (fork()) {
		dest.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		dest.sin_family = AF_INET;
		dest.sin_port = htons(BC_PORT);
		
		round_nb+=12;
		printf("sending BC\n");
		if (sendto(sock_bc, &round_nb, sizeof(round_nb), 0, (struct sockaddr *)&dest, sizeof(dest)) == -1) {
			perror("sendto");
			exit(1);
		}
	} else {
		if (recvfrom(sock_bc, &round_nb, sizeof(int), 0, (struct sockaddr*) &exp, (socklen_t*)&fromlen) == -1) { 
			perror("recvfrom");
			exit(2);
		}
		printf("<IP = %s, PORT = %d> -- %d \n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port), round_nb);
		
	}

		
	
	return 0;
	
}
