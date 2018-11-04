#define XOPEN_SOURCE 700

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int i, j;

void sig_hand(int sig) {
     printf("%d|%d receiving signal %d\n", i, j, sig);
}

int main (int arg, char * argv []){

     // change default behavior of SIGUSR1 & SIGUSR2
     sigset_t sig_mask;
     struct sigaction action;

     sigemptyset(&sig_mask);
     sigdelset(&sig_mask, SIGUSR1);
     sigdelset(&sig_mask, SIGUSR2);

     action.sa_mask = sig_mask;
     action.sa_flags = 0;
     action.sa_handler = sig_hand;

     sigaction(SIGUSR1, &action, 0);
     sigaction(SIGUSR2, &action, 0);
     // change default behavior of SIGUSR1 & SIGUSR2

     pid_t pid;
     pid_t sibling;

     j = 0;

     for (i = 0; (i < N) && (pid = fork()) != 0; i++)
          sibling = pid;

     printf ("%d i:%d j:%d \n", getpid(), i, j);

     // parent process waits for termination of all descendants
     if (pid > 0) {
          pause();
          pause();
          sleep(1);
     }

     // two child-less children wait for end of siblings
     if (((i % 2) == 0) && (pid == 0)) {
          pause();
          kill(getppid(), SIGUSR1 + i);
     }

     // two child-ful children
     if (((i % 2) == 1) && (pid == 0)) {
          while ((j < i) && ((pid = fork()) == 0)) {
               j++;
               printf ("%d i:%d j:%d \n", getpid(), i, j);
          }
          if (j == 0){
               pause();
               kill(sibling, SIGUSR1);
          } else {
               if (pid == 0) {
                    sleep(1);
                    kill(getppid(), SIGUSR1);
               } else {
                    pause();
                    kill(getppid(), SIGUSR1);
               }
          }
     }


     printf("%d|%d End of program\n", i, j);

     return EXIT_SUCCESS;
}