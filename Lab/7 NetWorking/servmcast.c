/** UDP Server on multi-cast address **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


#define MON_ADR_DIFF "225.0.0.10" /* multi-cast address */
#define PORTSERV 7200 /* Server port */


int main(int argc, char *argv[]) 
{
  int sock;
  struct sockaddr_in sin;        
  struct ip_mreq imr; /* Structure for setsockopt */
  char message[80];
 
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Cannot create datagram socket");
    exit(-1);
  }

  imr.imr_multiaddr.s_addr = inet_addr(MON_ADR_DIFF);
  imr.imr_interface.s_addr = INADDR_ANY;

  if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr,
                sizeof(struct ip_mreq)) == -1){
      perror("setsockopt");
      exit(1);
    }

  /* Create DNS "name" for server */
  memset((void*)&sin, 0, sizeof(sin));
   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORTSERV);
   sin.sin_family = AF_INET;

   /* bind name */
   if (bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
		perror("bind");
		exit(1);
	}
   /* Receive message */
   if ( recvfrom(sock,message,sizeof(message),0,NULL,NULL) == -1) { 
     perror("recvfrom");
     exit(2);
   }

   printf("Message received: %s\n", message);
   close(sock);
   return (0);

}

