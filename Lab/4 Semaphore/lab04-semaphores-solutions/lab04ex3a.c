/**
 * Solution A for the barrier pb
 * Uses 2 unnamed POSIX semaphores
 * _with_ a shared counter
 **/

#define XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5
#define IS_SHARED 1

typedef struct {
    sem_t sync;
    sem_t mx;
    int count;
} mysharedmem;

mysharedmem *mybarrier;


void wait_barrier(int nb_pcs) {
    int i;
    
    sem_wait(&(mybarrier->mx));
    mybarrier->count++;
    printf("%d> sem_count -- %d\n", getpid(), mybarrier->count);
    
    if (mybarrier->count == nb_pcs) {
        mybarrier->count = 0;
        sem_post(&(mybarrier->mx));
        for (i = 0; i < (nb_pcs-1); i++)
            sem_post(&(mybarrier->sync));
    } else {
        sem_post(&(mybarrier->mx));
        sem_wait(&(mybarrier->sync));
    }
}



int main(int argc , char **argv) {
    int fd, i;
    
    fd = shm_open("myshm", O_CREAT|O_RDWR, 0600);
    ftruncate(fd, sizeof(mysharedmem));
    mybarrier = (mysharedmem*)mmap(0, sizeof(mysharedmem), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    sem_init(&(mybarrier->sync), IS_SHARED, 0);
    sem_init(&(mybarrier->mx), IS_SHARED, 1);
    mybarrier->count = 0;
    
    for (i = 0; ((i < N) && (fork() > 0)); i++);
    
    printf("%d> Afore barrier\n", getpid());
    wait_barrier(N+1);
    printf("%d> After barrier\n", getpid());
    
    if (i < N)
        exit(0);
    
    for (i = 0; i < N; i++)
        wait(0);
    sem_destroy(&(mybarrier->mx));
    sem_destroy(&(mybarrier->sync));
    munmap(mybarrier, sizeof(mysharedmem));
    shm_unlink("myshm");

    return 0;
}
