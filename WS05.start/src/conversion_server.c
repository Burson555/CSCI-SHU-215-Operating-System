#include <conversion.h>

// $ conversion_server <server_id>
int i;
char* currencies[] = {"USD", "EUR", "JPY", "GBP"};
float rates[] = {0.143767, 0.126796, 16.3601, 0.110813};

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
    printf("Converting to %s\n", currencies[i]);
}

int main(int argc, char const *argv[])
{
	// open server shared memory
	int fd_server;
	int * sp_server;
    char shm_server[MAX_NAME_SIZE + 6];
    sprintf(shm_server, "%s_shm:0", argv[1]);
    fd_server = shm_open(shm_server, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_server, sizeof(info_struct));
    sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);

    // create child processes
    pid_t children[CURRENCY_NUMBER];
    for (i = 0; (i < CURRENCY_NUMBER) && ((children[i] = fork()) > 0); i++);

    // parent process loops with P's
    if (i == CURRENCY_NUMBER){
        char sem_server[MAX_NAME_SIZE + 6];
        sprintf(sem_server, "%s_sem:0", argv[1]);
        sem_t *prod;
        prod = sem_open(sem_server, O_CREAT|O_RDWR, 0666, 0);
        while (1){
            sem_wait(prod);
            sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);
            int currency_index = get_currency_index(sp_server->currency);
            kill(children[currency_index], SIGUSR1);
        }
    } else {
    	// set mask: only allow SIGUSR1
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
    	info_struct* info = malloc(sizeof(info_struct));
    	while (1){
    		pause();
    		// store info
		    info->server_id = sp_server->server_id;
			info->client_id = sp_server->client_id;
			info->currency = sp_client->currency;
			info->amount = sp_server->amount;
			// open client shared memory
			int fd_client;
			int * sp_client;
		    char shm_client[MAX_NAME_SIZE + 6];
		    sprintf(shm_client, "%s_shm:0", info->client_id);
		    fd_client = shm_open(shm_client, O_RDWR, 0);
		    ftruncate(fd_client, sizeof(info_struct));
		    sp_client = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
		    sp_client->amount = rates[i] * info->amount;
		    munmap(sp_client, sizeof(info_struct));
			// release the waiting client
			char sem_client[MAX_NAME_SIZE + 6];
		    sprintf(sem_client, "%s_sem:0", info->client_id);
		    sem_t *cons;
		    cons = sem_open(sem_client, O_RDWR);
		    sem_post(cons);
		    sem_close(sem_client);
		}
    }
}












