#define XOPEN_SOURCE 700

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define N 4

int i,j;

int main (int arg, char * argv []){

     pid_t pid;

     j = 0;

     for (i = 0; (i < N) && (pid = fork()) != 0; i++);

     printf ("i:%d j:%d \n", i, j);

     if (((i % 2) == 1) && (pid == 0)) {
          while ((j < i) && ((pid = fork()) == 0)) {
               j++;
               printf ("\ti:%d j:%d \n", i, j);
          }
     }

     printf("End of program\n");

     return EXIT_SUCCESS;
}