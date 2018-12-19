/** Example of multi-threaded TCP server */
/** Processes 4 client requests and then terminates **/

// before I write the socket list

#include "server.h"

int ticket = 0;
pthread_mutex_t mutex_ticket = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t mutex_node = PTHREAD_MUTEX_INITIALIZER;
int nodes[8400, 8401];


void* process_request(void* arg) {

    int sock_client;
    int service;
    sock_client = * (int*)arg;

    /*** Check service ***/
    if (read(sock_client, &service, sizeof(int)) < 0) {
        perror("read");
        exit(1);
    }

    /*** Process request ***/
    // DEPLOY ******************************************************************
    if ((int)service == DEPLOY) {
        printf("DEPLOY\n");
        pthread_mutex_lock(&mutex_ticket);
        int local_ticket;
        local_ticket = ticket;
        ticket ++;
        pthread_mutex_unlock(&mutex_ticket);

        /* Get nb_replicas */
        int nb_replicas;
        read(sock_client, &nb_replicas, sizeof(nb_replicas));
        printf("nb_replicas - %d\n", nb_replicas);
        /* Connect worker */
        struct sockaddr_in dest; /* Server address */
        struct addrinfo *result;
        int sock_worker;
        if ((sock_worker = socket(AF_INET,SOCK_STREAM,0)) == -1) {
            perror("socket");
            exit(1);
        }
            /* Find server address and use it to fill in structure dest */
        struct addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
        hints.ai_protocol = 0;
        if (getaddrinfo("localhost", 0, &hints, &result) != 0) {
            perror("getaddrinfo");
            exit(EXIT_FAILURE);
        }
        memset((void *)&dest, 0, sizeof(dest));
        memcpy((void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr,(void*)&dest.sin_addr,sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_port = htons(PORTWRKR);
            /* Establish connection */
        if (connect(sock_worker, (struct sockaddr *) &dest, sizeof(dest)) == -1) {
            perror("connect");
            exit(1);
        }
        /* Read from the client */
        int fd2;
        int or = BUFSZ;
        void* buffer = malloc(BUFSZ);
        char master_file[MAX_NAME_SIZE];
        sprintf(master_file, "master_file-%d", local_ticket);
        fd2 = open(master_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
        if (fd2 == -1) {
            perror("open");
            exit(3);
        }
        while(or == BUFSZ) {
            or = read(sock_client, buffer, BUFSZ);
            if (write(fd2, buffer, or) == -1) {
                perror("write_client");
                exit(1);
            }
            if (write(sock_worker, buffer, or) == -1) {
                perror("write_worker");
                exit(1);
            }
        }
        close(fd2);
        /* Finish exchange and return the ticket */
        printf("local_ticket = %d\n", local_ticket);
        if (write(sock_client, &local_ticket, sizeof(local_ticket)) == -1) {
               perror("write");
               exit(2);
        }
        /* Read stdout from the worker */
        or = BUFSZ;
        sprintf(master_file, "stdout-%d", local_ticket);
        fd2 = open(master_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
         while(or == BUFSZ) {
            or = read(sock_worker, buffer, BUFSZ);
            if (write(fd2, buffer, or) == -1) {
                perror("write_client");
                exit(1);
            }
        }
        /* Close connection */
        shutdown(sock_worker,2);
        close(sock_worker);
        /* Wait for the results from workers */

    // DEPLOY ******************************************************************
    // STATUS ******************************************************************
    } else if ((int)service == STATUS) {
        printf("STATUS\n");
    // STATUS ******************************************************************
    // RESULT ******************************************************************
    } else {
        printf("RESULT\n");
    }
    // RESULT ******************************************************************
    
    /* Close connection */
    shutdown(sock_client,2);
    close(sock_client);
    pthread_exit(0);
}



int main(int argc, char *argv[]){

    struct sockaddr_in sin;  /* Name of the connection socket (Server address) */
    struct sockaddr_in exp;  /* Client address */
    int sc ;                 /* Connection socket */
    int scom;             /* Communication socket */
    int fromlen = sizeof (exp);
    int i;
    pthread_t tid[MAX_CONNECTION];
    
    
    /* Create socket */
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket");
        exit(1); 
    }
    // printf("sc_master - %d\n", sc);
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
