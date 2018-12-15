/** Simple TCP client **/

#include "server.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in dest; /* Server address */
	struct addrinfo *result;
	int sock;
	int ticket;
	int service; 
	int reply;
	
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
	/* Send message (here it's an int)*/
	service = RESULT;		
	if (write(sock, &service, sizeof(service)) == -1) {
		perror("write");
		exit(1);
	}
	ticket = atoi(argv[2]);
	if (write(sock, &ticket, sizeof(ticket)) == -1) {
		perror("write");
		exit(1);
	}
	
	/* Receive reply */			
	if (read(sock, &reply, sizeof(reply)) == -1) {
		perror("read");
		exit(1);
	}

	/* Task not COMPLETED */
	if (reply != COMPLETED) {
		perror("Task not COMPLETED");
		exit(1);
	}
	
	/* Create file name */
	char file_name[MAX_NAME_SIZE] = "";
    sprintf(file_name, "stdoutTar-%d.tgz", ticket);
    char full_file[MAX_NAME_SIZE] = "";
    strcat(full_file, argv[3]);
    strcat(full_file, "/");
    strcat(full_file, file_name);

	/* Receive file */
    int fd1;
    int or;
    void* buffer = malloc(BUFSZ);
    or = BUFSZ;
    fd1 = open(file_name, O_RDWR|O_CREAT|O_TRUNC, 0600);
    while(or == BUFSZ) {
        or = read(sock, buffer, BUFSZ);
        if (write(fd1, buffer, or) == -1) {
            perror("write");
            exit(1);
        }
    }
    close(fd1);
    printf("Result retrieved: clt_file/stdoutTar-%d.tgz\n", ticket);

	/* Close connection */
	shutdown(sock,2);
	close(sock);
	return(0);
}