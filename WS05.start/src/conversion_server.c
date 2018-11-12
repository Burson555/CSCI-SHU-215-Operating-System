#include <conversion.h>

// $ conversion_server <server_id>
int i;
pid_t children[CURRENCY_NUMBER];
char* currencies[] = {"USD", "EUR", "JPY", "GBP"};
float rates[] = {0.143767, 0.126796, 16.3601, 0.110813};
char shm_server[MAX_NAME_SIZE + 6];
char sem_server[MAX_NAME_SIZE + 6];
char sem_server_mutex[MAX_NAME_SIZE + 11];
char sem_child_protect[MAX_NAME_SIZE + 19][CURRENCY_NUMBER];

int get_currency_index(char* currency){
	if (strncmp(currency, "USD", 3) == 0){
        return 0;}
    else if (strncmp(currency, "EUR", 3) == 0){
        return 1;}
    else if (strncmp(currency, "JPY", 3) == 0){
        return 2;}
    else if (strncmp(currency, "GBP", 3) == 0){
        return 3;}
    else{
        perror( "currency");
    }
    return -1;
}
void usr1Handler (int sig) {
    printf("Converting to %s by child %d\n", currencies[i], getpid());
}
void intHandler (int sig) {
	int j;
	for (j = 0; j < CURRENCY_NUMBER; j++){
	    printf("Killing child %d\n", children[j]);
		kill(children[j], SIGKILL);
	}
	printf("Destroying shm's and sem's\n");
    shm_unlink(shm_server);
    sem_unlink(sem_server);
    sem_unlink(sem_server_mutex);
    for (i = 0; i < CURRENCY_NUMBER; i++)
		sem_unlink(sem_child_protect[i]);
	printf("Killing PARENT %d\n", getpid());
    kill(getpid(), SIGKILL);
}

int main(int argc, char const *argv[]) {
	int currency_index;

	// open server shared memory
	int fd_server;
	info_struct * sp_server;
    sprintf(shm_server, "%s_shm:0", argv[1]);
    fd_server = shm_open(shm_server, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_server, sizeof(info_struct));
    sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);

    // mutex semaphore for this server
    // here the critical resource is server shared memory
    sprintf(sem_server_mutex, "%s_sem:mutex", argv[1]);
    sem_t *mutex;
    mutex = sem_open(sem_server_mutex, O_CREAT|O_RDWR, 0666, 1);

    // child protection semaphores
    // here the critical resource is the child lock memmory
    // if parent has to assign a working child new work
    // we have to make sure parent wait until this child finishes
    sem_t *child_protect[CURRENCY_NUMBER];
    for (i = 0; i < CURRENCY_NUMBER; i++){
    	// sem_child_protect 
    	sprintf(sem_child_protect[i], "%s_sem:child_protect", argv[1]);
	    child_protect[i] = sem_open(sem_child_protect[i], O_CREAT|O_RDWR, 0666, 1);
    }

    // create child processes
    for (i = 0; (i < CURRENCY_NUMBER) && ((children[i] = fork()) > 0); i++);

    // parent process loops with P's
    if (i == CURRENCY_NUMBER){
    	// set mask: only allow SIGINT
    	// uption receiving SIGINT: kill all children
    	sigset_t generalMask;
		sigfillset (& generalMask);
	    sigdelset (& generalMask, SIGINT);
    	sigprocmask (SIG_SETMASK, & generalMask, NULL);
	    struct sigaction actionINT;
	    actionINT.sa_mask = generalMask;
	    actionINT.sa_flags = 0;
	    actionINT.sa_handler = intHandler;
		sigaction (SIGINT, & actionINT, 0);
		// task of the parent
		// wait until a client sends the info of conversion
		// wake up and assign the task to a child
		// go back to waiting again
        sprintf(sem_server, "%s_sem:0", argv[1]);
        sem_t *prod;
        prod = sem_open(sem_server, O_CREAT|O_RDWR, 0666, 0);
        while (1){
        	printf("parent start waiting\n");
            sem_wait(prod);
            sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);
            currency_index = get_currency_index(sp_server->currency);
            sem_wait(child_protect[currency_index]);
            kill(children[currency_index], SIGUSR1);
        }
    } else {
    	// set mask: only allow SIGUSR1
    	// uption receiving SIGUSR1: awake and calculate
    	sigset_t generalMask;
		sigfillset (& generalMask);
	    sigdelset (& generalMask, SIGUSR1);
    	sigprocmask (SIG_SETMASK, & generalMask, NULL);
	    struct sigaction actionUSR1;
	    actionUSR1.sa_mask = generalMask;
	    actionUSR1.sa_flags = 0;
	    actionUSR1.sa_handler = usr1Handler;
		sigaction (SIGUSR1, & actionUSR1, 0);

		// create data structure and store currency info
    	info_struct* info = (info_struct*)malloc(sizeof(info_struct));
    	while (1){
        	printf("child %s start waiting\n", currencies[i]);
    		pause();
    		// store info and V the server mutex
		    strcpy(info->server_id, sp_server->server_id);
		    strcpy(info->client_id, sp_server->client_id);
		    strcpy(info->currency, sp_server->currency);
			info->amount = sp_server->amount;
			sem_post(mutex);
			// open client shared memory
			int fd_client;
			info_struct * sp_client;
		    char shm_client[MAX_NAME_SIZE + 6];
		    sprintf(shm_client, "%s_shm:0", info->client_id);
		    fd_client = shm_open(shm_client, O_RDWR, 0);
		    ftruncate(fd_client, sizeof(info_struct));
		    sp_client = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
    		float conversion_result = rates[i] * info->amount;
    		sp_client->amount = conversion_result;
		    munmap(sp_client, sizeof(info_struct));
			// release the waiting client
			char sem_client[MAX_NAME_SIZE + 6];
		    sprintf(sem_client, "%s_sem:0", info->client_id);
		    sem_t *cons;
		    cons = sem_open(sem_client, O_RDWR);
		    sem_post(cons);
		    sem_close(cons);
		    // release child protection
		    // this child can load other works
		    currency_index = get_currency_index(info->currency);
            sem_post(child_protect[currency_index]);
		}
    }
}












