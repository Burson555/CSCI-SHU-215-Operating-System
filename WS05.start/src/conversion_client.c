// do I need extra data structures?   YES
// leave the error detection to the last step   ---   ******************

// FIRST parse every input argument and store them into local variables
// i.e.,    char server_id[]
//          char client_id[]
//          char currency[]
//          float amount

// NOTE
//          1. client_id must be string so that server can locate the shm by name /<client_id>_shm:0
//          2. use another shared memory to store information like client_id

// SECOND create shared memory

#include <conversion.h>

int main(int argc, char *argv[]){

    // creating shared memory file
    int fd_client;
    info_struct * sp_client;
    char shm_client[MAX_NAME_SIZE + 6];
    sprintf(shm_client, "%s_shm:0", argv[2]);
    fd_client = shm_open(shm_client, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_client, sizeof(info_struct));
    sp_client = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);

    // constructing data structure of conversion information
    float init_value = atof(argv[4]);
    sp_client->amount = init_value;
    strcpy(sp_client->server_id, argv[1]);
    strcpy(sp_client->client_id, argv[2]);
    strcpy(sp_client->currency, argv[3]);
    // sp_client->amount = atof(argv[4]);

    // open server shared memory and write into it
    int fd_server;
    info_struct * sp_server;
    char shm_server[MAX_NAME_SIZE + 6];
    sprintf(shm_server, "%s_shm:0", argv[1]);
    fd_server = shm_open(shm_server, O_RDWR, 0);
    ftruncate(fd_server, sizeof(info_struct));
    sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);
    strcpy(sp_server->server_id, sp_client->server_id);
    strcpy(sp_server->client_id, sp_client->client_id);
    strcpy(sp_server->currency, sp_client->currency);
    sp_server->amount = sp_client->amount;

    // create client comsumption semaphore
    char sem_client[MAX_NAME_SIZE + 6];
    sprintf(sem_client, "%s_sem:0", argv[2]);
    sem_t *cons;
    cons = sem_open(sem_client, O_CREAT|O_RDWR, 0666, 0);

    // opening server semaphore and V the server
    char sem_server[MAX_NAME_SIZE + 6];
    sprintf(sem_server, "%s_sem:0", argv[1]);
    sem_t *prod;
    prod = sem_open(sem_server, O_RDWR);
    sem_post(prod);

    // P itself
    sem_wait(cons);

    // display the converion result
    printf("%f CNY ---> %f %s\n", init_value, sp_client->amount, sp_client->currency);
    munmap(sp_client, sizeof(info_struct));
    shm_unlink(shm_client);
    sem_close(cons);
    sem_unlink(sem_client);
    
    return 0;
}


