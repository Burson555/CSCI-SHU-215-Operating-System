#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	struct sockaddr_in sin;
	char host[64];
	
	memset((void*)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	sin.sin_family = AF_INET;
	
	if (getnameinfo((struct sockaddr*)&sin, sizeof(sin), host, sizeof(host), 0, 0, 0) != 0) {
		perror("getnameinfo");
        exit(EXIT_FAILURE);
    }
	
	printf("Name : %s\n", host);
	
	return (0);
}
