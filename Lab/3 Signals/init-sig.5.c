#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact_int;
struct sigaction sigact_alrm;
int n_int = 0;
int n_alrm = 0;

void sigalrm_handler (int sig) {
	printf ("reception de SIGALRM\n");
	alarm (1);
}

void sigint_handler (int sig) {
	printf ("reception de SIGINT\n");
}


int main () {
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigdelset (&sig, SIGALRM);

	sigprocmask (SIG_SETMASK, &sig, NULL);

	sigact_int.sa_handler = sigint_handler;
	sigaction (SIGINT, &sigact_int, NULL);

	sigact_alrm.sa_handler = sigalrm_handler;
	sigaction (SIGALRM, &sigact_alrm, NULL);

	printf("starting\n");
	alarm (5);

	while (1) {
		sigsuspend (&sig);
	}
	return 0;
}
