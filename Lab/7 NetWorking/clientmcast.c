/** Simple UDP client for multi-cast server **/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define MON_ADR_DIFF "225.0.0.10"
#define PORTSERV 7200

int main(int argc, char *argv[])
{
  struct sockaddr_in dest;
  int sock;
  char  message[80];

  
  if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Fill in structure for destination */  
  memset((void *)&dest,0, sizeof(dest));
  dest.sin_addr.s_addr = inet_addr(MON_ADR_DIFF);
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORTSERV);
	
  /* Write message ...*/
  strcpy(message,"MESSAGE DU CLIENT");

  /* Send message */
  if ( sendto(sock,message,strlen(message)+1,0,(struct sockaddr *)&dest,
	     sizeof(dest)) == -1) {
    perror("sendto");
    exit(1);
  }
  close(sock);
  return(0);
}



