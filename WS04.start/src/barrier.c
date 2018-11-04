#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int counter = -2;

void sig_hand(int sig) {
	counter ++;
	printf("\t%d receiving signal %d\n", getpid(), sig);
}

void calc1 () { 
	int i; 
	for (i = 0; i <1E8; i ++); 
} 

void calc2 () { 
	int i; 
	for (i = 0; i <1E8; i ++); 
} 

int main (int argc, char * argv []) { 
	// change default behavior of SIGUSR1 & SIGUSR2
	sigset_t sig_mask;
	struct sigaction action;

	sigemptyset(&sig_mask);

	action.sa_mask = sig_mask;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;

	sigaction(SIGUSR1, &action, 0);
	sigaction(SIGUSR2, &action, 0);
	// change default behavior of SIGUSR1 & SIGUSR2

	int i = 0; 
	pid_t pid_child [2]; 

	while ((i < 2) && ((pid_child [i] = fork ()) != 0)) 
		i ++; 

	printf("%d starting calc1\n", getpid());
	calc1 ();
	if (i < 2) {
		kill(getppid(), SIGUSR1 + i);
		pause();
	} else {
		while (counter != 0)
			pause();
		for (i = 0; i < 2; i++)
			kill(pid_child[i], SIGUSR1 + i);
	}

	printf("%d starting calc2\n", getpid());
	calc2 (); 

	if (i == 2) {
		wait(NULL);
		wait(NULL);
	}

	printf ( "End Process %d \n", getpid()); 

	return EXIT_SUCCESS; 

} 