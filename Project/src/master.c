/** Example of multi-threaded TCP server */
/** Processes 4 client requests and then terminates **/

#include "server.h"

int sum = 0;


void* process_request(void* arg) {
    // int *tmp;
    int sock;
    int signal;
    int fd2;
    int or = BUFSZ;
    void* buffer = malloc(BUFSZ);
    
    // tmp = (int*)arg;
    sock = * (int*)arg;

    /*** Read message ***/
    if (read(sock, &signal, sizeof(int)) < 0) {
        perror("read");
        exit(1);
    }
    
    /*** Process request ***/
    if ((int)signal == DEPLOY) {
        printf("DEPLOY\n");
        fd2 = open("haha", O_RDWR|O_CREAT|O_TRUNC, 0600);
        if (fd2 == -1) {
            perror("open");
            exit(3);
        }
        while(or == BUFSZ) {
            or = read(sock, buffer, BUFSZ);
            if (write(fd2, buffer, or) == -1) {
                perror("write");
                exit(1);
            }
        }
        close(fd2);
    } else if ((int)signal == STATUS) {
        printf("STATUS\n");
    } else {
        printf("RESULT\n");
    }
    
    sum += signal;
    printf("sum = %d\n", sum);
    if (write(sock, &sum, sizeof(sum)) == -1) {
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
    int scom;             /* Communication socket */
    int fromlen = sizeof (exp);
    int i;
    pthread_t tid[5];
    
    
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
    listen(sc, MAX_CONNECTION);
    
    /* Main loop */
    for (i = 0;i < MAX_CONNECTION - 1; i++) {
        if ( (scom = accept(sc, (struct sockaddr *)&exp, (socklen_t *) &fromlen))== -1) {
            perror("accept");
            exit(2);
        }
        
        /* Create a thread to handle the newly connected client */
        int* tscom = (int*)malloc(sizeof(int));
        *tscom = scom;
        pthread_create((pthread_t*)&tid[i], 0, process_request, (void *) tscom);
        
    }
    
    for (i = 0;i < MAX_CONNECTION - 1; i++)
    pthread_join((pthread_t) tid[i], 0);
    
    close(sc);
    
    return 0;
}
