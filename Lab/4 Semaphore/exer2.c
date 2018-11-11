#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#define N 5


int * sp;
int main () {
	int fd;
	int i, j;
	int sum = 0;

	// / * Create segment myshm * /
	if ((fd = shm_open ( "myshm", O_RDWR | O_CREAT, 0600)) == -1) {
		perror ( "shm_open");
		exit (1);
	}

	// / * Allocate enough memory to store ten integers * /
	if (ftruncate (fd, N * sizeof (int)) == -1) {
		perror ( "ftruncate");
		exit (1);
	}

	// / * Map the segment in shared memory to process * /
	if ((sp = (int *) mmap (NULL, N * sizeof (int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror ( "mmap");
		exit(1);
	}

    // / * Creation of semaphore mutexes* /

    // Loop of parent
    		// P1(await child do) -- loop needed
    		// reap the integers
    		// V2(allow child do again) -- loop needed

    // Loop of children
    // 			P2(await approval from parent)
    // 			generate random integers
    // 			V1(allow parent to reap)

    sem_t * semprod[N];
    sem_t * semcons[N];
    char* sem_name = (char*) malloc(16 * sizeof(char));
    for (i = 0; i < N; i++){
    	sprintf(sem_name, "semprod%d", i);
        printf("semaphore name == %s\n", sem_name);
	    if ((semprod[i] = sem_open ( sem_name, O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED) { 
	        if (errno != EEXIST) {
	            perror ( "sem_open"); 
	            exit (1); 
	        }
	        semprod[i] = sem_open(sem_name, O_RDWR);
	    }
	    sprintf(sem_name, "semcons%d", i);
        printf("semaphore name == %s\n", sem_name);
	    if ((semcons[i] = sem_open ( sem_name, O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
	        if (errno != EEXIST) {
	            perror ( "sem_open"); 
	            exit (1); 
	        }
	        semcons[i] = sem_open(sem_name, O_RDWR);
	    }
    }
	
	for (i = 0; (i < N) && (fork() > 0); i++);

	if (i < N){
		for (j = 0; j < N; j++) {
			sem_wait(semprod[i]);
			srand(getpid() * rand());
			sp[i] = rand() % 10;
			printf("%d-Child %d | Number %d: %d\n", getpid(), i, j, sp[i]);
			sem_post(semcons[i]);
		}
		return 0;
	} // child processes end here
	else {
		for (j = 0; j < N; j++){
			for (i = 0; i < N; i++)
				sem_wait(semcons[i]);
			for (i = 0; i < N; i++) {
                printf("PP> child %d - %d\n", i, sp[i]);
				sum += sp[i];
			}
			printf("Parent says: the sum is %d\n", sum);
			for (i = 0; i < N; i++)
				sem_post(semprod[i]);
		}
	}

	// Await children
	for (i = 0; i < N; i++)
		wait(NULL);

	// "Detach" segment
	munmap (sp, N * sizeof (int));

	// Destroy the segment
	shm_unlink ("myshm");

	// Close & Destroy the semaphore
    for (i = 0; i < N; i++){
		sem_close(semprod[i]);
		sem_close(semcons[i]);
    	sprintf(sem_name, "semprod%d", i);
		sem_unlink(sem_name);
    	sprintf(sem_name, "semcons%d", i);
		sem_unlink(sem_name);
    }

	return 0;
}
