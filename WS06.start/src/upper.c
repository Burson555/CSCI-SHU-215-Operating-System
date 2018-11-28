#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count;
void* convert(void *argv){
    FILE *fp1, *fp2;
    char dest_fname[128];
    int c = 1;
    
    fp1 = fopen ((char*)argv, "r");
    strcpy(dest_fname, (char*)argv);
    strcat(dest_fname, ".UPPER.txt");
    printf("%s\n", dest_fname);
    fp2 = fopen (dest_fname, "w");
    if ((fp1 == NULL) || (fp2 == NULL)) {
        perror ( "fopen");
        exit (1);
    }
    
    while (c != EOF) {
        c = fgetc(fp1);
        if (c != EOF){
            fputc(toupper(c), fp2);
            // printf("%d -> %d\n", c, toupper(c));
        }
//            fputc(toupper(c), stdout);
    }
    
    fclose (fp1);
    fclose (fp2);
    pthread_mutex_lock(&mutex);
    count--;
    if (count == 0){
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit((void*)0);
}


int main (int argc, char ** argv) {
    
    int i;
    count = argc - 1;
    pthread_t tid[count];

    pthread_mutex_lock(&mutex);
    for (i = 1; i < argc; i++){
        pthread_create(&tid[i], NULL, convert, argv[i]);
    }
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}
