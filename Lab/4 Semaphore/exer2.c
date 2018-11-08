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
    sem_t * semcons;
    char sem_name[N][15];
    if ((semP = sem_open ( "semP", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
        if (errno != EEXIST) {
            perror ( "sem_open"); 
            exit (1); 
        }
        semP = sem_open("semP", O_RDWR);
    }
    for (i = 0; i < N; i++){
    	char nb[3] = "";
    	itoa(i, nb, 10);
    	strcat(sem_name[i], "sem");
    	strcat(sem_name[i], nb);
	    if ((semprod[i] = sem_open ( sem_name[i], O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
	        if (errno != EEXIST) {
	            perror ( "sem_open"); 
	            exit (1); 
	        }
	        semprod[i] = sem_open(sem_name[i], O_RDWR);
	    }
    }
	
	for (i = 0; (i < N) && (fork() > 0); i++);

	if (i < N){
		for (j = 0; j < N; j++) {
			sem_wait(semprod[i]);
			srand(getpid() * rand());
			sp[i] = rand() % 10;
			printf("%d-Child %d | Number %d: %d\n", getpid(), i, j, sp[i]);
			sem_post(semprod[i]);
		}
		return 0;
	} // child processes end here
	else {
		for (i = 0; i < N; i++)
			sem_post(semprod[i]);
		for (j = 0; j < N; j++){
			for (i = 0; i < N; i++)
				sem_wait(semP);
			for (i = 0; i < N; i++)
				sum += sp[i];
			printf("Parent says: the sum is %d\n", sum);
			for (i = 0; i < N; i++)
				sem_post(semprod[i]);
		}
	}

	printf("Parent says: the sum is %d\n", sum);

	// Await children
	for (i = 0; i < N; i++)
		wait(NULL);

	// "Detach" segment
	munmap (sp, N * sizeof (int));

	// Destroy the segment
	shm_unlink ("myshm");

	// Close the semaphore
    sem_close (semP);
    for (i = 0; i < N; i++)
		sem_close(semprod[i]);
    // Destroy the semaphore
    sem_unlink ("semP");
    for (i = 0; i < N; i++)
		sem_unlink(sem_name[i]);


 //    sem_t * semP, * semC;
 //    if ((semP = sem_open ( "semP", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
 //        if (errno != EEXIST) {
 //            perror ( "sem_open"); 
 //            exit (1); 
 //        }
 //        semP = sem_open("semP", O_RDWR);
 //    }
 //    if ((semC = sem_open ( "semC", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) { 
 //        if (errno != EEXIST) {
 //            perror ( "sem_open"); 
 //            exit (1); 
 //        }
 //        semC = sem_open("semC", O_RDWR);
 //    }

	// for (i = 0; (i < N) && (fork() > 0); i++);

	// if (i < N){
	// 	for (j = 0; j < N; j++) {
	// 		sem_wait(semC);
	// 		srand(getpid() * rand());
	// 		sp[i] = rand() % 10;
	// 		printf("%d-Child %d | Number %d: %d\n", getpid(), i, j, sp[i]);
	// 		sem_post(semP);
	// 	}
	// 	return 0;
	// } // child processes end here
	// else {
	// 	for (i = 0; i < N; i++)
	// 		sem_post(semC);
	// 	for (j = 0; j < N; j++){
	// 		for (i = 0; i < N; i++)
	// 			sem_wait(semP);
	// 		for (i = 0; i < N; i++)
	// 			sum += sp[i];
	// 		printf("Parent says: the sum is %d\n", sum);
	// 		for (i = 0; i < N; i++)
	// 			sem_post(semC);
	// 	}
	// }

	// printf("Parent says: the sum is %d\n", sum);

	// // Await children
	// for (i = 0; i < N; i++)
	// 	wait(NULL);

	// // "Detach" segment
	// munmap (sp, N * sizeof (int));

	// // Destroy the segment
	// shm_unlink ("myshm");

	// // Close the semaphore
 //    sem_close (semP);
 //    sem_close (semC);
 //    // Destroy the semaphore
 //    sem_unlink ("semP");
 //    sem_unlink ("semC");

	return 0;
}

// each cell of the shared memory 
// only contains the sum of N random integers the process created
// the random integers are local to the child process but not the parent
