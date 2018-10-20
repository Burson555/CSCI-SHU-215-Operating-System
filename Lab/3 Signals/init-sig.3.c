#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact;

void sig_hand(int sig){
	printf("hahahaha %d\n", sig);
}

int main () {
	sigemptyset (&sig);
	// sigaddset (&sig, SIGINT);
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	// sigact.sa_handler = SIG_IGN;
	sigact.sa_handler = sig_hand;
	sigaction (SIGALRM, &sigact, NULL);
	alarm (3);
	// printf("start sleeping\n");
	// sleep(4);
	// printf("start sleeping\n");
	sigsuspend (&sig);
	printf ("APRES sigsuspend\n");
	return 0;
}
