/** Simple TCP client **/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORTSERV 7200

int main(int argc, char *argv[])
{
	struct sockaddr_in dest; /* Server address */
	struct addrinfo *result;
	int sock;
	int msg; 
	int reply;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s machine\n", argv[0]);
		exit(1);
	}
	
	if ((sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	/* Find server address and use it to fill in structure dest */
	
	struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    hints.ai_protocol = 0;
	
	if (getaddrinfo(argv[1], 0, &hints, &result) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
    }
	memset((void *)&dest,0, sizeof(dest));
	memcpy((void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr,(void*)&dest.sin_addr,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORTSERV);
	
	/* Establish connection */
	if (connect(sock, (struct sockaddr *) &dest, sizeof(dest)) == -1) {
		perror("connect");
		exit(1);
	}
	msg = 10;				
	/* Send message (here it's an int)*/
	if (write(sock,&msg,sizeof(msg)) == -1) {
		perror("write");
		exit(1);
	}
	
	/* Receive reply */			
	if (read(sock,&reply,sizeof(reply)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	printf("reply : %d\n", reply);
	
	/* Close connection */
	shutdown(sock,2);
	close(sock);
	return(0);
}
