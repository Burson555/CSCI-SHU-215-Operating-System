#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    struct addrinfo *result, *rp;
	
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    hints.ai_protocol = 0;
	
	if (getaddrinfo(argv[1], 0, 0, &result) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
    }
	
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        printf("%d - %d - %d\n", rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		printf("address - %s\n", inet_ntoa(((struct sockaddr_in*)rp->ai_addr)->sin_addr));
		printf("canon name - %s\n", rp->ai_canonname);
	}
	
    freeaddrinfo(result);     /* No longer needed */
	
	return (0);
}
