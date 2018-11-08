#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#define N 10



int * sp;
int main () {
	int fd;
	int i;

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

	for (i = 0; (i < N) && (fork() > 0); i++);

	if ( i < N){
		srand(getpid());
		sp[i] = rand() % 10;
		printf("%d | Child %d: %d\n", getpid(), i, sp[i]);
		return 0;
	}
	// child processes end here

	// Await children
	for (i = 0; i < N; i++)
		wait(NULL);

	// Access segment
	int sum = 0;
	for (i = 0; i < N; i++)
		sum += sp[i];
	printf("Parent says: the sum is %d\n", sum);

	// "Detach" segment
	munmap (sp, N * sizeof (int));

	// Destroy the segment
	shm_unlink ("myshm");
	return 0;
}