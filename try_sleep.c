#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#define N 10



// void sigint_handler (int sig) {
// 	printf ("reception de SIGINT\n");
// }

int main () {
	if (fork() > 0) {
		sleep(3);
	} else {
		sleep(1);
		// kill(getppid(), SIGINT);
		kill(getppid(), SIGWINCH);
		// kill(getppid(), SIGINT);
		// kill(getppid(), SIGINT);
	}
	return 0;
}