#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact;
int n = 0;

void sigint_handler (int sig) {
	printf ("n = %d; Signal received: %d\n", n, sig);
	if (n == 0) {
		sigact.sa_handler = SIG_IGN;
		sigaction (SIGINT, &sigact, NULL);
		n = 1;
	}
	else {
		sigact.sa_handler = SIG_DFL;
		sigaction (SIGINT, &sigact, NULL);
	}
}

int main () {
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);

	sigprocmask (SIG_SETMASK, &sig, NULL);

	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = sigint_handler;

	sigaction (SIGINT, &sigact, NULL);
	
	printf ("AVANT sigsuspend\n");
	sigsuspend (&sig);
	printf ("APRES 1er sigsuspend\n");
	sigsuspend (&sig);
	printf ("APRES 2e sigsuspend\n");
	sigsuspend (&sig);
	printf ("APRES 3e sigsuspend\n");
	return 0;
}
