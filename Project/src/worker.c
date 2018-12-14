/** Simple TCP client **/

#include "server.h"

void* process_request(void* arg) {
    printf("Upon recrption\n");

    int fd2;
    int or = BUFSZ;
    void* buffer = malloc(BUFSZ);
    int sock_master;
    sock_master = * (int*)arg;

    char worker_file[MAX_NAME_SIZE];

    sprintf(worker_file, "worker_file-%d", sock_master);
    fd2 = open(worker_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
    if (fd2 == -1) {
        perror("open");
        exit(3);
    }
    while(or == BUFSZ) {
        or = read(sock_master, buffer, BUFSZ);
        if (write(fd2, buffer, or) == -1) {
            perror("write");
            exit(1);
        }
    }
    close(fd2);
    /* Now I have the code */
    /* Run the code */
    pid_t pid;
    if ((pid = fork()) > 0) {
    	/* Code for parent: wait(pid) and wait only */
    	printf("parent\n");
    } else {
		/* Unzip the compressed file */
		/* Call make in the directory */
		/* Locate the stdout file */
		printf("child\n");
		exit(0);
    }
    /* Send back the stdout file */
    or = BUFSZ;
    sprintf(worker_file, "worker_file-%d", sock_master);
    fd2 = open(worker_file, O_RDWR, 0600);
    if (fd2 == -1) {
        perror("open");
        exit(3);
    }
    while(or == BUFSZ) {
        or = read(fd2, buffer, BUFSZ);
        if (write(sock_master, buffer, or) == -1) {
            perror("write_client");
            exit(1);
        }
    }
    close(fd2);
    /* Close connection */
    shutdown(sock_master,2);
    close(sock_master);
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    
    // networking variables
    struct sockaddr_in sin;  /* Name of the connection socket (Server address) */
    struct sockaddr_in exp;  /* Master address */
    int sc ;                 /* Connection socket */
    int scom;             /* Communication socket */
    int fromlen = sizeof (exp);
    int i;
    int port;
    port = atoi(argv[1]);
    pthread_t tid[MAX_CONNECTION];
    
    /* Create socket */
	if (argc != 2) {
		fprintf(stderr, "Invalid input\n");
		exit(1);
	}
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket");
        exit(1); 
    }
    // printf("sc_worker - %d\n", sc);
    memset((void*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
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
