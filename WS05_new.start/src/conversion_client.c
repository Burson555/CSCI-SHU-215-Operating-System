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

char* currencies[] = {"USD", "EUR", "JPY", "GBP", "CNY"};
float rates[] = {USD_rate, EUR_rate, JPY_rate, GBP_rate, CNY_rate};

int get_currency_index(char* currency){
    if (strncmp(currency, "USD", 3) == 0){
        return 0;}
    else if (strncmp(currency, "EUR", 3) == 0){
        return 1;}
    else if (strncmp(currency, "JPY", 3) == 0){
        return 2;}
    else if (strncmp(currency, "GBP", 3) == 0){
        return 3;}
    else if (strncmp(currency, "CNY", 3) == 0){
        return 4;}
    else{
        perror("currency");
    }
    return -1;
}

int main(int argc, char *argv[]){

    // creating shared memory file
    int i;
    int currency_type;

    int fd_client_array;
    float * flt_array;
    char shm_client_array[MAX_NAME_SIZE + 6];
    sprintf(shm_client_array, "%s_shm:0", argv[2]);
    fd_client_array = shm_open(shm_client_array, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_client_array, CURRENCY_NUMBER * sizeof(float));
    flt_array = (float*)mmap(0, CURRENCY_NUMBER * sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client_array, 0);

    // constructing data structure of conversion information
    currency_type = get_currency_index(argv[3]);
    float init_value = atof(argv[4]);

    // open server shared memory and write into it
    int fd_server;
    info_struct * sp_server;
    char shm_server[MAX_NAME_SIZE + 6];
    sprintf(shm_server, "%s_shm:0", argv[1]);
    fd_server = shm_open(shm_server, O_RDWR, 0);
    ftruncate(fd_server, sizeof(info_struct));
    sp_server = (info_struct*)mmap(0, sizeof(info_struct), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);

    
    // P the server mutex and store the data "safely"
    char sem_server_mutex[MAX_NAME_SIZE + 11];
    sprintf(sem_server_mutex, "%s_sem:mutex", argv[1]);
    sem_t *mutex;
    mutex = sem_open(sem_server_mutex, O_RDWR);
    sem_wait(mutex);
    strcpy(sp_server->server_id, argv[1]);
    strcpy(sp_server->client_id, argv[2]);
    strcpy(sp_server->currency, argv[3]);
    sp_server->amount = init_value;

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
    for (i = 0; i < CURRENCY_NUMBER; i++){
        if (i != currency_type)
            printf("%f %s ---> %f %s\n", init_value, argv[3], flt_array[i], currencies[i]);
    }
    printf("\n");

    munmap(flt_array, sizeof(info_struct));
    shm_unlink(shm_client_array);
    sem_close(cons);
    sem_unlink(sem_client);
    
    return 0;
}


