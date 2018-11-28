#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define NB_THREAD 5
#define MAX_FILES 12
#define MAX_FNAME 118

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *count;
int nb_files;
char file_arr[MAX_FILES][MAX_FNAME];

void* convert(void *argv){
    int index;
    pthread_mutex_lock(&mutex);
    while (*count < nb_files){
        index = *count;
        *count = *count + 1;
        pthread_mutex_unlock(&mutex);

        FILE *fp1, *fp2;
        char dest_fname[128];
        int c = 1;
        
        fp1 = fopen ((char*)file_arr[index], "r");
        strcpy(dest_fname, (char*)file_arr[index]);
        strcat(dest_fname, ".UPPER.txt");
        printf("ST> %ld ---> %s\n", (long)pthread_self(), dest_fname);
        fp2 = fopen (dest_fname, "w");
        if ((fp1 == NULL) || (fp2 == NULL)) {
            perror ( "fopen");
            exit (1);
        }
        
        while (c != EOF) {
            c = fgetc(fp1);
            if (c != EOF){
                fputc(toupper(c), fp2);
            }
               // fputc(toupper(c), stdout);
        }
        
        fclose (fp1);
        fclose (fp2);

        pthread_mutex_lock(&mutex);
        if (*count >= nb_files)
            pthread_mutex_unlock(&mutex);
    }
    pthread_exit((void*)0);
}


int main (int argc, char ** argv) {

    int i;
    int return_value;
    nb_files = argc - 1;
    count = (int*)malloc(sizeof(int));
    pthread_t tid[nb_files];
    for (i = 0; i < argc - 1; i ++)
        strcpy(file_arr[i], argv[i + 1]);
    pthread_mutex_lock(&mutex);
    for (i = 0; i < NB_THREAD; i++){
        pthread_create(&tid[i], NULL, convert, (void*) count);
        if (i == argc - 2)
            break;
    }
    *count = 0;
    pthread_mutex_unlock(&mutex);
    for (i = 0; (i < NB_THREAD) && (i < argc - 1); i++){
        if(pthread_join(tid[i], (void**)&return_value) != 0) {
            perror("join");
            exit(1);
        }
        printf("MT> tid[%d] = %ld\n", i, (long)tid[i]);
    }

    return EXIT_SUCCESS;
}
