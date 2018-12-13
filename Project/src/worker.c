/** Simple TCP client **/

#include "server.h"

int main(int argc, char *argv[]){
    
    // networking variables
    struct sockaddr_in sin;  /* Name of the connection socket (Server address) */
    struct sockaddr_in exp;  /* Master address */
    int sc ;                 /* Connection socket */
    int scom;             /* Communication socket */
    int fromlen = sizeof (exp);
    int i;
    
    /* Create socket */
	// if (argc != 2) {
	// 	fprintf(stderr, "Invalid input\n");
	// 	exit(1);
	// }
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket");
        exit(1); 
    }
    
    memset((void*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORTWRKR);
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
        
        /* Start working */
        printf("Upon recrption\n");
        int fd2;
        int or = BUFSZ;
        void* buffer = malloc(BUFSZ);

        char worker_file[MAX_NAME_SIZE];
        sprintf(worker_file, "worker_file-%d", PORTWRKR);

        fd2 = open(worker_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
        if (fd2 == -1) {
            perror("open");
            exit(3);
        }
        while(or == BUFSZ) {
            or = read(scom, buffer, BUFSZ);
            if (write(fd2, buffer, or) == -1) {
                perror("write");
                exit(1);
            }
        }
        close(fd2);
    }
    
    close(sc);
    
    return 0;
}
