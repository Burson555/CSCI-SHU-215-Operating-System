#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>




#define N 5


int main(int argc , char **argv) {
    
    int i, total = 0, fd, *vals;
    
    
    fd = shm_open("myshm", O_CREAT|O_RDWR, 0600);
    ftruncate(fd, N*sizeof(int));
    vals = (int*)mmap(0, N*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    
    
    for (i = 0; ((i < N) && (fork() > 0)); i++)
        ;
    
    if (i < N) {
        srand(getpid());
        vals[i] = rand()%10;
        printf("%d(%d) > %d\n", getpid(), i, vals[i]);
        munmap(vals, N*sizeof(int));
        exit(0);
    } else {
        for (i = 0; i < N; i++)
            wait(0);
        for (i = 0; i < N; i++) {
            printf("PP> child %d - %d\n", i, vals[i]);
            total += vals[i];
        }
    }

    printf("PP> Total sum of values received is %d\n", total);
    munmap(vals, N*sizeof(int));
    shm_unlink("myshm");
    return 0;
}
