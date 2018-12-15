/** Example of multi-threaded TCP server */
/** Processes 4 client requests and then terminates **/

#include "server.h"

int ticket = 0;
pthread_mutex_t mutex_ticket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_node = PTHREAD_MUTEX_INITIALIZER;
int nodes[NB_NODE];
int nodes_counter = 0;
int ticket_status[MAX_CONNECTION];


void* process_request(void* arg) {
    
    int i;

    int sock_client;
    int service;
    sock_client = * (int*)arg;

    /*** Check service ***/
    printf("check service\n");
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
        ticket_status[local_ticket] = RUNNING;

        /* Get nb_replicas */
        int nb_replicas;
        read(sock_client, &nb_replicas, sizeof(nb_replicas));
        int assigned_nodes[nb_replicas];
        /* Assign nodes to the deployment */
        pthread_mutex_lock(&mutex_node);
        for (i = 0; i < nb_replicas; i++){
            assigned_nodes[i] = nodes[nodes_counter];
            nodes_counter = (nodes_counter + 1)%NB_NODE;
        }
        pthread_mutex_unlock(&mutex_node);
        /* Connect worker */
        struct sockaddr_in dest; /* Server address */
        struct addrinfo *result;
        int sock_worker[nb_replicas];
        for (i = 0; i < nb_replicas; i++){
            if ((sock_worker[i] = socket(AF_INET,SOCK_STREAM,0)) == -1) {
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
            dest.sin_port = htons(assigned_nodes[i]);
                /* Establish connection */
            if (connect(sock_worker[i], (struct sockaddr *) &dest, sizeof(dest)) == -1) {
                perror("connect");
                exit(1);
            }
            printf("Get worker %d by port %d\n", i + 1, sock_worker[i]);
        }
        /* Read from the client */
        int fd2;
        int or = BUFSZ;
        void* buffer = malloc(BUFSZ);
        char source_file[MAX_NAME_SIZE];
        sprintf(source_file, "mst_file/sourceFile-%d", local_ticket);
        fd2 = open(source_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
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
            for (i = 0; i < nb_replicas; i++){
                if (write(sock_worker[i], buffer, or) == -1) {
                    perror("write_worker");
                    exit(1);
                }
            }
        }
        close(fd2);
        /* Finish exchange and return the ticket */
        printf("JOB TICKET %d\n", local_ticket);
        if (write(sock_client, &local_ticket, sizeof(local_ticket)) == -1) {
               perror("write");
               exit(2);
        }
        /* Read stdout from the worker */
        struct stat st = {0};
        char dir_name[MAX_NAME_SIZE];
        sprintf(dir_name, "mst_file/stdoutFolder-%d", local_ticket);
        if (stat(dir_name, &st) == -1) {
            mkdir(dir_name, 0700);
        }
        for (i = 0; i < nb_replicas; i++){
            or = BUFSZ;
            sprintf(source_file, "%s/stdout-%d-%d", dir_name, local_ticket, i);
            fd2 = open(source_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
             while(or == BUFSZ) {
                or = read(sock_worker[i], buffer, BUFSZ);
                if (write(fd2, buffer, or) == -1) {
                    perror("write_client");
                    exit(1);
                }
            }
            close(fd2);
        }
        ticket_status[local_ticket] = COMPLETED;
        /* Close connection */
        for (i = 0; i < nb_replicas; i++){
            shutdown(sock_worker[i], 2);
            close(sock_worker[i]);
        }
        /* Wait for the results from workers */

    // DEPLOY ******************************************************************
    // STATUS ******************************************************************
    } else if ((int)service == STATUS) {
        printf("STATUS\n");
        /* Get target_ticket */
        int target_ticket;
        read(sock_client, &target_ticket, sizeof(target_ticket));
        write(sock_client, &ticket_status[target_ticket], sizeof(ticket_status[target_ticket]));
    // STATUS ******************************************************************
    // RESULT ******************************************************************
    } else {
        printf("RESULT\n");
        /* Get target_ticket */
        int target_ticket;
        read(sock_client, &target_ticket, sizeof(target_ticket));
        /* Check directory --- too complicated */ 
        // DIR *pt_Dir;
        // struct dirent* dirEnt;
        // struct stat info;
        // char stdouts[NB_NODE][MAX_NAME_SIZE];
        // int counter = 0;
        // if ((pt_Dir = opendir(".") ) == NULL) {
        //     perror ("opendir");
        //     return EXIT_FAILURE;
        // }
        // while ((dirEnt = readdir(pt_Dir)) !=NULL) {
        //     stat(dirEnt->d_name, &info);
        //     if (S_ISREG(info.st_mode)) {
        //         char file_identifier[MAX_NAME_SIZE];
        //         sprintf(file_identifier, "stdout-%d", target_ticket)
        //         if (strncmp(dirEnt->d_name, file_identifier, strlen(file_identifier)) == 0)
        //             stdouts[counter++] = dirEnt->d_name;
        //     }
        // }
        // closedir (pt_Dir);
        /* Now I have a array of all requested stdout files */
        /* Check whether the job is completed */
        printf("check complete\n");
        if (ticket_status[target_ticket] != COMPLETED) {
            write(sock_client, &ticket_status[target_ticket], sizeof(ticket_status[target_ticket]));
            shutdown(sock_client, 2);
            close(sock_client);
            pthread_exit(0);
        } else {
            write(sock_client, &ticket_status[target_ticket], sizeof(ticket_status[target_ticket]));
        }
        /* Generate the full directory name */
        printf("full dir name\n");
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
           perror("getcwd");
           exit(2);
        }
        char dir_name[MAX_NAME_SIZE];
        sprintf(dir_name, "mst_file/stdoutFolder-%d", target_ticket);
        char full_dir[MAX_NAME_SIZE] = "";
        strcat(full_dir, cwd);
        strcat(full_dir, "/");
        strcat(full_dir, dir_name);
        /* Generate the full compressed file name */
        printf("fll file name\n");
        char file_name[MAX_NAME_SIZE] = "";
        sprintf(file_name, "mst_file/stdoutTar-%d.tgz", target_ticket);
        // char full_file[MAX_NAME_SIZE] = "";
        // strcat(full_file, cwd);
        // strcat(full_file, "/");
        // strcat(full_file, file_name);
        /* Compress file */
        printf("tar tar tar\n");
        if (fork() > 0)
            wait(NULL);
        else
            execlp("tar", "tar", "-czvf", file_name, dir_name, NULL);
            // execlp("tar", "tar", "-czvf", full_file, full_dir, NULL);
        /* Send to client */
        printf("to client\n");
        int fd1;
        int or;
        void* buffer = malloc(BUFSZ);
        or = BUFSZ;
        fd1 = open(file_name, O_RDWR, 0600);
        while(or == BUFSZ) {
            or = read(fd1, buffer, BUFSZ);
            if (write(sock_client, buffer, or) == -1) {
                perror("write---");
                exit(1);
            }
        }
        close(fd1);
    }
    // RESULT ******************************************************************
    
    /* Close connection */
    shutdown(sock_client, 2);
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
    
    /* Initialize ticket status array */
    for (i = 0; i < MAX_CONNECTION; i++)
        ticket_status[i] = INVALID;
    
    /* Initialize the ndoe array */
    for (i = 0; i < NB_NODE; i++)
        nodes[i] = 8400 + i;

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
    printf("Master server successfully launched\n");
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
