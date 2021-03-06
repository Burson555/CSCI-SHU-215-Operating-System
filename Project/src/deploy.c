/** Simple TCP client **/

#include "server.h"


int main(int argc, char *argv[]){

	struct sockaddr_in dest; /* Server address */
	struct addrinfo *result;
	int sock;
	int service; 
	int ticket;

	// file I/O variables
    int fd1;
    int or;
    int nb_replicas;
    struct stat info1;
    void* buffer = malloc(BUFSZ);
	
	// $deploy <server_name> <#replicas> <compressed_file>
	if (argc != 4) {
		fprintf(stderr, "Invalid input\n");
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
	// send the file to master server
	/* tell the master this is a deploy request */
	service = DEPLOY;		
	if (write(sock, &service, sizeof(service)) == -1) {
		perror("write");
		exit(1);
	}
	nb_replicas = atoi(argv[2]);
	if (write(sock, &nb_replicas, sizeof(nb_replicas)) == -1) {
		perror("write");
		exit(1);
	}
	// start sending file to socket
	or = BUFSZ;
    fd1 = open(argv[3], O_RDWR, 0600);
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
		if (write(sock, buffer, or) == -1) {
			perror("write");
			exit(1);
		}
    }
    close(fd1);

	// **********************************************************
	
	// **********************************************************
	/* Receive ticket */			
	// below I should be receiving job ticket (int) from the server
	// in all cases where the deployment isn't successful
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


