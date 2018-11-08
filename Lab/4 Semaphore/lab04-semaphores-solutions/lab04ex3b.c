/**
 * Solution B for the barrier pb
 * Uses 2 unnamed POSIX semaphores
 * _without_ shared counter
 * Adds a cool cleanup feature
 **/

#define XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5
#define IS_SHARED 1

typedef struct {
    sem_t sync1;
    sem_t sync2;
} mysharedmem;

mysharedmem *mybarrier;


void wait_barrier(int pcs_nb) {
    int i;
    if (pcs_nb == N) {
        for (i = 0; i < N; i++)
            sem_wait(&(mybarrier->sync1));
        for (i = 0; i < N; i++)
            sem_post(&(mybarrier->sync2));
    } else {
        //sem_post(&(mybarrier->sync1));
        sem_wait(&(mybarrier->sync2));
    }
}

void cleanup() {
    sem_destroy(&(mybarrier->sync1));
    sem_destroy(&(mybarrier->sync2));
    munmap(mybarrier, sizeof(mysharedmem));
    shm_unlink("myshm");
}


int main(int argc , char **argv) {
    int fd, i;
    
    sigset_t mask;
    struct sigaction act;
    
    sigfillset(&mask);
    act.sa_handler = cleanup;
    act.sa_mask = mask;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    
    fd = shm_open("myshm", O_CREAT|O_RDWR, 0600);
    ftruncate(fd, sizeof(mysharedmem));
    mybarrier = (mysharedmem*)mmap(0, sizeof(mysharedmem), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    sem_init(&(mybarrier->sync1), IS_SHARED, 0);
    sem_init(&(mybarrier->sync2), IS_SHARED, 0);
    
    for (i = 0; ((i < N) && (fork() > 0)); i++);
    
    
    printf("%d> Afore barrier\n", getpid());
    
    wait_barrier(i);
    
    printf("%d> After barrier\n", getpid());
    
    if (i < N)
        exit(0);
    
    for (i = 0; i < N; i++)
        wait(0);
    
    cleanup();
    
    return 0;
}
