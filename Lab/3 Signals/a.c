#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

// Page 11 example
// int main (int arg, char ** argv) {
//     printf ( "begin program\n");
//     kill (getpid (), SIGINT);
//     printf ( "end program\n");
//     return 0;
// }

// Page 15 example sigaction
// void sig_hand(int sig) {
//    printf ( "received signal% d \n", sig);
// }

//  int main (int argc, char ** argv) {
//          sigset_t sig_proc;
//          struct sigaction action;

//          printf ( "start program \n");
//          sigemptyset(&sig_proc);
//          action.sa_mask = sig_proc;
//          action.sa_flags = 0;
//          action.sa_handler = sig_hand;
//          sigaction (SIGINT, &action, 0);
//          kill (getpid(), SIGINT);
//          printf ( "end program \n");
//  }

// Page 24 example mask
// int main(int argc, char const *argv[]) {
//   sigset_t sig_proc;
//   printf("start program\n");
//
//   sigemptyset(&sig_proc);
//   sigaddset(&sig_proc, SIGINT);
//   sigprocmask(SIG_BLOCK, &sig_proc, 0);
//
//   sleep(5);
//   printf("after sleep\n");
//   sigprocmask(SIG_UNBLOCK, &sig_proc, 0);
//
//   printf("end program\n");
//   sleep(6);
//   return 0;
// }

// Page 26 example pending signals
// int main (int argc, char * argv []) { 
//   sigset_t sig_set, wait_set;
//   // / * List of blocked signals * / 
//   sigemptyset (& sig_set); 
//   sigemptyset (& wait_set); 
//   sigaddset (& sig_set, SIGINT); 
//   // / * Hide * SIGINT / 
//   sigprocmask (SIG_SETMASK, & sig_set, NULL); 
//   printf("start killing\n");
//   kill(getpid (), SIGINT); 
//   printf("end killing\n");
//   // / * Get the list of pending signals * /
//   printf("print pending #1\n\n");
//   sigpending (& wait_set); 
//   printf("\nprint pending #2\n");
//   if (sigismember (& wait_set, SIGINT)) 
//     printf ( "SIGINT is pending\n"); 
//   sleep(5);
//   printf("hahahah\n");
//   return EXIT_SUCCESS; 
// }

// Page 29 example waiting for signal
// void sig_hand (int sig) {
// printf ( "\nreceived signal% d \n\n", sig);
// }

// int main (int argc, char ** argv) {
//   sigset_t sig_proc;
//   struct sigaction action;

//   // / * Mask SIGINT * / 
//   sigemptyset (& sig_proc); 
//   sigaddset (& sig_proc, SIGINT); 
//   // sigprocmask (SIG_SETMASK, & sig_proc, NULL); 
//   // in this step we do mask SIGINT
//   // because we entered the set to sigprocmask
//   printf("1rd\n");

//   // / * Redefine handler for SIGINT * / 
//   // sa_mask is not a set but a pointer
//   // thus changing sig_proc will auto change sa_mask value
//   action.sa_mask = sig_proc; 
//   action.sa_flags = 0; 
//   action.sa_handler = sig_hand; 
//   sigaction (SIGINT, & action, NULL); 

//   sleep(3);
//   // / * Wait for SIGINT * / 
//   printf(" before changing sig_proc\n");
//   printf("changing sig_proc: sigfillset\n");
//   sigfillset (& sig_proc);
//   printf("changing phase 1: sigdelset\n");
//   sigdelset (& sig_proc, SIGINT); 
//   printf("changing phase 2: sigsuspend\n");
//   alarm(3);
//   sigsuspend (& sig_proc); 
//   printf("finish changing\n");

//   printf("right before termination\n");
//   sleep(4);
//   return EXIT_SUCCESS;
// }

// Page 32 example timer notification
void sig_hand (int sig) {
  printf ( "received signal% d \n", sig);
}
 int main (int argc, char ** argv) {
   sigset_t sig_proc;
   struct sigaction action;
   sigemptyset (& sig_proc);
   action.sa_mask = sig_proc;
   action.sa_flags = 0;
   action.sa_handler = sig_hand;
   sigaction (SIGALRM, & action, 0);
   while (1) {
     alarm(1);
     pause ();
  }
  return EXIT_SUCCESS;
}

// Page 37 example loss of pending signals
// int cont;
// void sig_hand (int sig) {
//   if (sig == SIGUSR1){
//     cont ++;
//     printf ( "cont++ \n");
//   }
//   else {
//     printf ( "number of SIGUSR1 received: %d \n", cont);
//     exit (0);
//   }
// }

// pid_t pid_child;
// int main (int argc, char ** argv) {
//     sigset_t sig_proc;
//     int i;
//     struct sigaction action;
//     sigemptyset (& sig_proc);
//     action.sa_mask = sig_proc;
//     action.sa_flags = 0;
//     action.sa_handler = sig_hand;
//     sigaction (SIGUSR1, & action, 0);
//     sigaction (SIGINT, & action, 0);
//     if ((pid_child = fork ()) == 0) {
//       while (1)
//         pause ();
//     } else {
//       for (i = 0; i < 20; i ++)
//         kill (pid_child, SIGUSR1);
//       kill (pid_child, SIGINT);
//       wait (NULL);
//       printf("child process ended\n");
//       return EXIT_SUCCESS;
//  }
// }
