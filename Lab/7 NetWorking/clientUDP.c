/** Simple UDP client **/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define PORTSERV 4567 /* Server port */
#define TAILMSG 80

int main(int argc, char *argv[])
{
	int reply;
	struct sockaddr_in dest;
	int sock;
	unsigned int fromlen = sizeof(dest);
	char  message[TAILMSG];
	struct addrinfo *result;
	
	
	/* Server name and port nb are passed as arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage : %s machine \n", argv[0]);
		exit(1);
	}
	
	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	
    /* Find server address and use it to fill in structure dest */
	
	struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    hints.ai_protocol = 0;
	
	if (getaddrinfo(argv[1], 0, &hints, &result) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
    }
	
	printf("address - %s\n", inet_ntoa(((struct sockaddr_in*)result->ai_addr)->sin_addr));
	printf("canon name - %s\n", result->ai_canonname);
	
	
	memset((void *)&dest,0, sizeof(dest));
	memcpy((void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr,(void*)&dest.sin_addr,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORTSERV);
	
	/* Write message ...*/
	strcpy(message,"MESSAGE DU CLIENT");
	
	/* Send message */
	if ( sendto(sock,message,strlen(message)+1,0,(struct sockaddr *)&dest,
				sizeof(dest)) == -1) {
		perror("sendto");
		exit(1);
	}
	
	/* Receive reply */
	if ( recvfrom(sock,&reply,sizeof(reply),0,0,&fromlen) == -1) {
		perror("recvfrom");
		exit(1);
	}
	printf("reply : %d\n", reply);
	close(sock);
	
	return(0);
}
