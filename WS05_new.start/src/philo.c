#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#define MAX_NAME_SIZE 32

void calc1 () {
    int j;
    srand(getpid() * rand());
    for (j = 0; j < 1E18 * (rand() % 10); j ++);
}

int main(int argc, char **argv) {
    // if (argc != 3) {
    //     printf("argments error\n");
    //     return 0;
    // }
    int fd;
    int N, round;
    int i, j, k;
    int * chopsticks;
    N = atoi(argv[1]);
    round = atoi(argv[2]);

    sem_t **sem_chop, **sem_philo;
    sem_chop = (sem_t **)malloc(N * sizeof(sem_t *));
    sem_philo = (sem_t **)malloc(((int)ceil(N / 2)) * sizeof(sem_t *));

    char sem_chop_name[MAX_NAME_SIZE + 10], sem_philo_name[MAX_NAME_SIZE + 9];

    if ((fd = shm_open("myshm", O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm open error");
        exit(1);
    }
    if (ftruncate(fd, N * sizeof(int)) == -1) {
        perror("trunc error");
        exit(1);
    }
    chopsticks = (int *)mmap(0, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (i = 0; i < N; i++) {
        chopsticks[i] = 1;
        sprintf(sem_chop_name, "/chop_sem:%d", i);
        if ((sem_chop[i] = sem_open(sem_chop_name, O_CREAT | O_RDWR | O_EXCL, 0666, 1)) == SEM_FAILED) {
            perror("sem_open");
            exit(1);
        };
        if ((i % 2) == 0) {
            sprintf(sem_philo_name, "/phi_sem:%d", i / 2);
            if ((sem_philo[i / 2] = sem_open(sem_philo_name, O_CREAT | O_RDWR, 0666, 1)) == SEM_FAILED) {
                perror("sem_open");
                exit(1);
            };
        }
    }

    for (i = 0; (i < N) && (fork() > 0); i++);

    if (i != N) {
        for (k = 0; k < round; k++) {
            sem_wait(sem_philo[(int)ceil(i / 2)]);
            sem_wait(sem_chop[i]);
            sem_wait(sem_chop[(i + 1) % N]);
            sem_post(sem_philo[(int)ceil(i / 2)]);
            chopsticks[i] = 0;
            chopsticks [(i + 1)% N] = 0;
            // calc1();
            sleep(1);
            for (j = 0; j < N; j++) {
                printf("%d ", chopsticks[j]);
            }
            printf("\n");
            chopsticks[i] = 1;
            chopsticks [(i + 1)% N] = 1;
            sem_post(sem_chop[i]);
            sem_post(sem_chop[(i + 1) % N]);
        }
    }
    else if (i == N) {
        for (i = 0; i < N; i++) {
            wait(NULL);
        }
        for (i = 0; i < N; i++) {
            sem_close(sem_chop[i]);
            sprintf(sem_chop_name, "/chop_sem:%d", i);
            sem_unlink(sem_chop_name);
            if ((i % 2) == 0) {
                sem_close(sem_philo[i / 2]);
                sprintf(sem_philo_name, "/phi_sem:%d", i / 2);
                sem_unlink(sem_philo_name);
            }
        }
        printf("All done\n");
        free(sem_chop);
        free(sem_philo);
        munmap(chopsticks, N * sizeof(int));
        shm_unlink("myshm");
    }
    return 0;
}