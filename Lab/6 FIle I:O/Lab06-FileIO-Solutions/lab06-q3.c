#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define N 4

int main(int argc, char** argv) {
    
    int pid, fd, i;
    int buffer, total = 0;
    
    fd = open("./xchg", O_RDWR|O_CREAT|O_TRUNC|O_SYNC, 0600);
    if (fd == -1) {
        perror("Pb à l'ouverture du fichier");
        exit(3);
    }
    
    /* creation des fils */
    for (i = 0; ((i < N) && ((pid = fork()) > 0)); i++)
        ;
    
    if (pid == 0) {
        srand(getpid());
        buffer = (rand()%10)+1;
        printf("%d> val = %d\n", getpid(), buffer);
        if (write(fd, &buffer, sizeof(int)) == 0)
            perror("write zero");
        close(fd);
        exit(0);
    }
    
    for (i = 0; i < N; i++)
        wait(0);
    
    if (lseek(fd, SEEK_SET, 0) == -1)
        perror("lseek");
    
    for (i = 0; i < N; i++) {
        if (read(fd, &buffer, sizeof(int)) == 0)
            perror("read zero");
        printf("%d> INTER = %d\n", getpid(), buffer);
        total += buffer;
    }
    
    printf("%d> TOTAL = %d\n", getpid(), total);
    
    close(fd);
    unlink("./xchg");
    return 0;
    
}
