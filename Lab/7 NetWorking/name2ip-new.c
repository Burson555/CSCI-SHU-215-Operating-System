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
    struct addrinfo *result;
	
	if (getaddrinfo(argv[1], 0, 0, &result) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
    }
	
	printf("address - %s\n", inet_ntoa(((struct sockaddr_in*)result->ai_addr)->sin_addr));
	
    freeaddrinfo(result);     /* No longer needed */
	
	return (0);
}
