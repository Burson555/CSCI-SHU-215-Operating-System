/** Simple TCP client **/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define PORTSERV 8080
#define BUFSZ 1024

#define DEPLOY 0
#define STATUS 1
#define RESULT 2

int main(int argc, char *argv[])
{
	int i;

	struct sockaddr_in dest; /* Server address */
	struct addrinfo *result;
	int sock;
	int service; 
	int ticket;

	// file I/O variables
    int fd1;
    int or;
    // int fileSZ;
    struct stat info1;
    void* buffer = malloc(BUFSZ);
	
	if ((argc != (atoi(argv[1]) + 2)) || (argc < 4)) {
		fprintf(stderr, "Missing files\n");
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

// **********************************************************
	// below I should send the files to the master server
	service = DEPLOY;		
	/* tell the master this is a deploy request */
	if (write(sock,&service,sizeof(service)) == -1) {
		perror("write");
		exit(1);
	}
	for (i = 2; i < argc; i++){

		or = BUFSZ;
		// fileSZ = 0;

	    fd1 = open(argv[i], O_RDWR, 0600);
	    if (fd1 == -1) {
	        perror("open");
	        exit(1);
	    }
	    fstat(fd1, &info1);
	    if (!(S_ISREG(info1.st_mode))) {
	        perror("regular file");
	        exit(1);
	    }
	    
	    while(or == BUFSZ) {
	        or = read(fd1, buffer, BUFSZ);
	        // fileSZ += or;
			if (write(sock, buffer, or) == -1) {
				perror("write");
				exit(1);
			}
	    }
	    
	    close(fd1);
	}
	// **********************************************************
	
	// **********************************************************
	/* Receive ticket */			
	// below I should be receiving job ticket from the server
	// I don't have to write a struct for that
	// because the ticket number is an integer
	// in all cases where the deployment isn't successfukl
	// the master server returns a -1
	if (read(sock, &ticket, sizeof(ticket)) == -1) {
		perror("read");
		exit(1);
	}
	printf("Job deployment successful - JOB TICKET %d\n", ticket);
	// **********************************************************
	
	/* Close connection */
	shutdown(sock,2);
	close(sock);
	return(0);
}
