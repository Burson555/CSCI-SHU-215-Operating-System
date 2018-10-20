#define XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>




int main (int argc, char** argv) {
    int pid, i;
    int nb_children;
    
    nb_children = argc - 1;
    for (i = 0; ((i < (nb_children)) && ((pid = fork()) > 0)); i++) {
    }
    
    if (pid > 0) {
        printf("PARENT> Preparing link edition\n");
        char* execv_args[nb_children + 4];
        execv_args[0] = (char*) malloc(sizeof("gcc"));
        memcpy(execv_args[0], "gcc", strlen("gcc"));
        execv_args[1] = (char*) malloc(sizeof("-o"));
        memcpy(execv_args[1], "-o", strlen("-o"));
        execv_args[2] = (char*) malloc(sizeof("my_exec"));
        memcpy(execv_args[2], "my_exec", strlen("my_exec"));
        for (i = 0; i < nb_children; i++) {
            execv_args[i + 3] = (char*) malloc(sizeof(argv[i + 1]));
            strcpy(execv_args[i + 3], argv[i + 1]);
            execv_args[i + 3][strlen(execv_args[i + 3]) - 1] = 'o';
            printf("%s\n", execv_args[i + 3]);
        }
        execv_args[nb_children + 3] = NULL;
        printf("PARENT> Waiting for children\n");
        for (i = 0; i < nb_children; i++) {
            wait(0);
        }
        printf("PARENT> All children are finished => Editing links\n");
        execvp("gcc", execv_args);
    } else {
        char* filename = argv[i + 1];
        printf("CHILD %d> Compiling %s\n", i, filename);
        execlp("gcc", "gcc", filename, "-c", NULL);
        printf("CHILD %d> Complete\n", i);
    }
    
    return EXIT_SUCCESS;
}