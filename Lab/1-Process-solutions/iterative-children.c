#define XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N_CHILDREN 4

int main (int argc, char** argv) {
    int pid, i;
    for (i = 0; ((i < N_CHILDREN) && ((pid = fork()) > 0)); i++) {
    }
    if (pid > 0) {
        for (i = 0; i < N_CHILDREN; i++) {
            if (i == 0) {
                waitpid(pid, 0, 0);
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