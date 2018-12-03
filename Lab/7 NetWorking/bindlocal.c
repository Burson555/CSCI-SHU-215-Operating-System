#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, const char **argv)
{
	int sock;
	struct sockaddr addr;

	addr.sa_family = AF_UNIX;
	strcpy(addr.sa_data, "MySocket");	

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error: socket creation");
		exit(1);
	}
			
	if (bind(sock, (struct sockaddr *) &addr, strlen(addr.sa_data)) == -1) {
		perror("Error: naming");
		exit(2);
	} 	
	return (0);
}