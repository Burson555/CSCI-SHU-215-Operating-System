/** Simple TCP client **/

#include "server.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
int port;

void* process_request(void* arg) {

    int fd2;
    int or = BUFSZ;
    void* buffer = malloc(BUFSZ);
    int sock_master;
    sock_master = * (int*)arg;

	char stdout_worker[MAX_NAME_SIZE];
    char source_file[MAX_NAME_SIZE];
	pthread_mutex_lock(&mutex);
    int local_counter;
    local_counter = counter;
    counter ++;
    pthread_mutex_unlock(&mutex);
    sprintf(source_file, "wkr_file/source_file-%d", local_counter);
    fd2 = open(source_file, O_RDWR|O_CREAT|O_TRUNC, 0600);
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
    	wait(NULL);
    } else {
		/* Generate the full directory name */
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
		   perror("getcwd");
		   exit(2);
		}
		struct stat st = {0};
    	char dir_name[MAX_NAME_SIZE];
    	sprintf(dir_name, "wkr_file/extractFolder-%d-%d", port, local_counter);
		if (stat(dir_name, &st) == -1) {
		    mkdir(dir_name, 0700);
		}
    	char full_dir[MAX_NAME_SIZE] = "";
    	strcat(full_dir, cwd);
    	strcat(full_dir, "/");
    	strcat(full_dir, dir_name);
		/* Generate the full file name */
    	char full_file[MAX_NAME_SIZE] = "";
    	strcat(full_file, cwd);
    	strcat(full_file, "/");
    	strcat(full_file, source_file);
		/* Unzip the compressed file */
		if ((pid = fork()) > 0) {
	    	wait(NULL);
	    } else {
	    	execlp("tar", "tar", "xvzf", full_file, "-C", full_dir, NULL);
	    }
		/* Locate the stdout file */
		int fd1;
    	sprintf(stdout_worker, "wkr_file/stdout_worker-%d-%d", port, local_counter);
		if ((fd1 = open (stdout_worker, O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) { 
			perror ( "open \n"); 
			exit(2);
		}
		dup2(fd1, STDOUT_FILENO);
		/* Call make in the directory */
		execlp("make", "make", "-C", full_dir, NULL);
		exit(0);
    }
    /* Send back the stdout file */
    or = BUFSZ;
    sprintf(stdout_worker, "wkr_file/stdout_worker-%d-%d", port, local_counter);
    fd2 = open(stdout_worker, O_RDWR, 0600);
    if (fd2 == -1) {
        perror("open");
        exit(3);
    }
    while(or == BUFSZ) {
        or = read(fd2, buffer, BUFSZ);
        if (write(sock_master, buffer, or) == -1) {
            perror("write");
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
    printf("Worker server successfully launched\n");
    for (i = 0;i < MAX_CONNECTION - 1; i++) {
        if ( (scom = accept(sc, (struct sockaddr *)&exp, (socklen_t *) &fromlen))== -1) {
            perror("accept");
            exit(2);
        }
	    // printf("%s: Upon recrption - scom: %d\n", argv[1], scom);
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
