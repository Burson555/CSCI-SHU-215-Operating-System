/** Example of a simple UDP server **/

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

#define PORTSERV 4567
#define TAILMSG 80

int main(int argc, char *argv[])
{
	struct sockaddr_in sin;  /* Server address */
	struct sockaddr_in exp;	/* Client address */
	int sc;
	uint fromlen = sizeof(exp);
	int cpt = 0; 
	char host[64];
	char message[TAILMSG];
	
	/* Create socket */
	if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("creation socket");
		exit(1);
	}
	
	/* Prepare server name and bind to DNS service */
	memset((void*)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORTSERV);
	sin.sin_family = AF_INET;
	if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind");
		exit(2);
	}
	/*** Receive message ***/
	if ( recvfrom(sc, message, sizeof(message), 0, (struct sockaddr *) &exp, &fromlen) == -1) { 
		perror("recvfrom");
		exit(2);
	}
	/*** Display client address ***/
	
	printf("Sender: <IP = %s, PORT = %d> \n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port));
	
	/* Name de the client machine */
	if (getnameinfo((struct sockaddr*)&exp, sizeof(exp), host, sizeof(host), 0, 0, 0) != 0) {
		perror("getnameinfo");
        exit(EXIT_FAILURE);
    }
	
	printf("Machine : %s\n", host);
	
	/*** Process message ***/
	printf("Message : %s\n", message); 
	cpt++;
	
	/*** Send reply ***/
	if (sendto(sc,&cpt,sizeof(cpt),0,(struct sockaddr *)&exp,fromlen) == -1) {
		perror("sendto");
		exit(2);
	}
	close(sc);
	return (0);
}
