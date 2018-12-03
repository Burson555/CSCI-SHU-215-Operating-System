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

#define NAMESZ 128

int main(int argc, char *argv[])
{
	char host[NAMESZ];
    struct addrinfo *result;
	
	gethostname(host, NAMESZ);
	printf("host\t\t%s\n", host);
	
	if (getaddrinfo(host, 0, 0, &result) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
    }
	
	printf("address\t\t%s\n", inet_ntoa(((struct sockaddr_in*)result->ai_addr)->sin_addr));
	
    freeaddrinfo(result);     /* No longer needed */
	
	return (0);
}
