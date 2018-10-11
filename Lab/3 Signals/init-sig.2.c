#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;

int main () {
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigprocmask (SIG_SETMASK, &sig, NULL);
	sigsuspend (&sig);
	printf ("1er signal\n");
	sigsuspend (&sig);
	printf ("2e signal\n");
	return 0;
}
