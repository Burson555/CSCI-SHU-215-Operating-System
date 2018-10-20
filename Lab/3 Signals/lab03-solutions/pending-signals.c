#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv []) {
    
    int i;
    sigset_t  sig_rec ;	/* list of blocked singals */
    sigset_t  sig_pend;	/* list of pending signals */
    
    
    sigfillset(&sig_rec);
    sigprocmask (SIG_SETMASK, &sig_rec, NULL);

    sigemptyset (&sig_rec);
    sigaddset (&sig_rec,SIGINT);
    sigaddset (&sig_rec,SIGQUIT);
    
    /* mask SIGINT and SIGQUIT */
    sigprocmask (SIG_SETMASK, &sig_rec,NULL);
    
    sleep (30);
    
    /* retrieve pending signals */
    sigpending (&sig_pend);
    if (sigismember ( &sig_pend,SIGINT) )
        printf("SIGINT received\n");
    if (sigismember ( &sig_pend,SIGQUIT) )
        printf("SIGQUIT received\n");
    
    for (i=1; i <NSIG; i++)
        if (sigismember ( &sig_pend,i) )
            printf("%d",i);
    printf ("\n");

    /**
    sigdelset (&sig_rec,SIGINT);
    sigdelset (&sig_rec,SIGQUIT);
    sigprocmask (SIG_SETMASK, &sig_rec,NULL);
    **/
    
    printf ("End program \n");
    
    return EXIT_SUCCESS;
}