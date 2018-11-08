#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>

#define N 5

void calc1 () {
    int i;
    for (i = 0; i <1E8; i ++);
}

void calc2 () {
    int i;
    for (i = 0; i <1E8; i ++);
}

int main (int argc, char * argv []) {

    int i = 0;
    pid_t pid_child [N - 1];

    // / * Creation of semaphore mutexes* /
    sem_t * smutex1, * smutex2;
    if ((smutex1 = sem_open ( "mysem1", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
        if (errno != EEXIST) {
            perror ( "sem_open"); 
            exit (1); 
        }
        // / * Semaphore already created, open without O_CREAT * / 
        smutex1 = sem_open("mysem1", O_RDWR);
    }
    if ((smutex2 = sem_open ( "mysem2", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
        if (errno != EEXIST) {
            perror ( "sem_open"); 
            exit (1); 
        }
        // / * Semaphore already created, open without O_CREAT * / 
        smutex2 = sem_open("mysem2", O_RDWR);
    }

    while ((i < N - 1) && ((pid_child [i] = fork ()) != 0))
        i ++;

    calc1 ();
    printf("%d Finish calc1\n", i);
    if (i == N - 1){
        for (i = 0; i < N - 1; i++)
            sem_wait (smutex1);
        for (i = 0; i < N - 1; i++)
            sem_post (smutex2);
    } else {
        sem_post (smutex1);
        sem_wait (smutex2);
    }

    calc2 ();
    printf ( "%d End Process\n", i);

    // Close the semaphore
    sem_close (smutex1);
    sem_close (smutex2);
    // Destroy the semaphore
    sem_unlink ("mysem1");
    sem_unlink ("mysem2");

    return 0;
}
