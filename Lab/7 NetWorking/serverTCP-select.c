/** Example of multi-threaded TCP server */
/** Processes 4 client requests and then terminates **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>


#define PORTSERV 7200

int cpt, sum = 0;


int process_request(int sock) {
    /*** Read message ***/
    if (read(sock,&cpt, sizeof(cpt)) < 0) {
        perror("read");
        exit(1);
    }
    
    /*** Process message ***/
    
    sum += cpt;
    printf("sum = %d\n", sum);
    if (write(sock, &sum, sizeof(cpt)) == -1) {
	       perror("write");
	       exit(2);
    }
    
    /* Close connection */
    shutdown(sock,2);
    close(sock);
    pthread_exit(0);
}



int main(int argc, char *argv[])
{
    struct sockaddr_in sin;  /* Name of the connection socket (Server address) */
    struct sockaddr_in exp;  /* Client address */
    int sc ;                 /* Connection socket */
    int scom;		      /* Communication socket */
    int fromlen = sizeof (exp);
    int i;
    int tid[5];
    
    
    /* Create socket */
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket");
        exit(1);
    }
    
    memset((void*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORTSERV);
    sin.sin_family = AF_INET;
    
    // lease the address if someone else wanna use it
    // not directly sent to router 
    int reuse = 1;
    setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    
    /* Register server on DNS svc */
    if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind");
        exit(2);
    }
    
    // five connections at most
    // not sent to router
    listen(sc, 5);
    
    /* Main loop */
    
    for (i = 0;i < 4; i++) {
        if ( (scom = accept(sc, (struct sockaddr *)&exp, &fromlen))== -1) {
            perror("accept");
            exit(2);
        }
        
        /* Create a thread to handle the newly connected client */
        int* tscom = (int*)malloc(sizeof(int));
        *tscom = scom;
        pthread_create(&tid[i], 0, process_request, tscom);
        
    }
    
    for (i = 0;i < 4; i++)
    pthread_join(tid[i], 0);
    
    close(sc);
    
    return 0;
}
