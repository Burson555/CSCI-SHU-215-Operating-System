
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTSERV 8080
#define MAX_CONNECTION 9999

#define BUFSZ 1024

#define COMPLETED 0
#define RUNNING 1
#define INVALID 2

#define DEPLOY 0
#define STATUS 1
#define RESULT 2
