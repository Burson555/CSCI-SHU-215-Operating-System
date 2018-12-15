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
	
	if (argc != 3) {
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
	service = STATUS;		
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

	switch(reply) {

		case COMPLETED:
			printf("JOB#%s - COMPLETED\n", argv[2]);
			break;

		case RUNNING:
			printf("JOB#%s - RUNNING\n", argv[2]);
			break;

		case INVALID:
			printf("JOB#%s - INVALID\n", argv[2]);
			break;

		default:
			printf("Please run the program again\n");

}
	
	/* Close connection */
	shutdown(sock,2);
	close(sock);
	return(0);
}