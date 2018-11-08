#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>




#define N 3


int main(int argc , char **argv) {

	int i, j, total = 0, fd, *vals;
    sem_t *scons[N];
    sem_t *sprod[N];
	
	
	fd = shm_open("myshm", O_CREAT|O_RDWR, 0600);
	ftruncate(fd, N*sizeof(int));
	vals = (int*)mmap(0, N*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    char* s_name = (char*) malloc(16*sizeof(char));
    for (i = 0; i < N; i++) {
        sprintf(s_name, "/scons:%d", i);
        printf("semaphore name == %s\n", s_name);
        scons[i] = sem_open(s_name, O_CREAT|O_RDWR, 0600, 0);
        if (scons[i] == SEM_FAILED) {
            perror("sem_open");
            exit(1);
        }
        sprintf(s_name, "/sprod:%d", i);
        printf("semaphore name == %s\n", s_name);
        sprod[i] = sem_open(s_name, O_CREAT|O_RDWR, 0600, 1);
        if (sprod[i] == SEM_FAILED) {
            perror("sem_open");
            exit(2);
        }
    }
	
	
	for (i = 0; ((i < N) && (fork() > 0)); i++);
	
	if (i < N) {
        srand(getpid());
        for (j = 0; j < N; j++) {
            sem_wait(sprod[i]);
            vals[i] = rand()%10;
            printf("%d(%d) > %d\n", getpid(), i, vals[i]);
            sem_post(scons[i]);
        }
		exit(0);
	} else {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++)
                sem_wait(scons[i]);
            for (i = 0; i < N; i++) {
                printf("PP> child %d - %d\n", i, vals[i]);
                total += vals[i];
            }
            for (i = 0; i < N; i++)
                sem_post(sprod[i]);
        }
	}

    for (i = 0; i < N; i++) {
		wait(0);
        sem_close(sprod[i]);
        sprintf(s_name, "/sprod:%d", i);
        sem_unlink(s_name);
        sem_close(scons[i]);
        sprintf(s_name, "/scons:%d", i);
        sem_unlink(s_name);
    }

    printf("PP> Total sum of values received is %d\n", total);

	return 0;
}
