#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact;

int main () {
	sigemptyset (&sig);
	sigaddset (&sig, SIGINT);
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = SIG_IGN;
	sigaction (SIGALRM, &sigact, NULL);
	alarm (5);
	sigsuspend (&sig);
	printf ("APRES sigsuspend\n");
	return 0;
}
