#define XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N_CHILDREN 4

int pid;

void create_child(int how_many) {
    if ((how_many > 0) && ((pid = fork()) > 0))
        create_child(how_many - 1);
}


int main (int argc, char** argv) {


    create_child(N_CHILDREN);
    
    if (pid > 0)
        printf("Parent\n");
    else
        printf("Child\n");
    
    if (pid > 0) {
        for (i = 0; i < N_CHILDREN; i++) {
            if (i == 0) {
                //waitpid
                printf("waited for last created child\n");
            }
            if (i == 1) {
                wait(0);
                printf("waited for any child\n");
            }
            wait(0);
        }
        printf("All children are finished - Parent\n");
    } else
        printf("Child\n");

    
    
    return EXIT_SUCCESS;
}