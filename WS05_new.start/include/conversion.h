#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define USD 0
#define EUR 1
#define JPY 2
#define GBP 3
#define CNY 4

#define USD_rate 0.143767
#define EUR_rate 0.126796
#define JPY_rate 16.3601
#define GBP_rate 0.110813
#define CNY_rate 1

#define MAX_LINE_SIZE 512
#define MAX_NAME_SIZE 32
#define CURRENCY_LENGTH 5
#define CURRENCY_NUMBER 5

typedef struct info_struct {
    char server_id[MAX_NAME_SIZE]; // server_id
    char client_id[MAX_NAME_SIZE]; // client_id
    char currency[CURRENCY_LENGTH]; // currency type
    float amount; // how much to convert
} info_struct;

int get_currency_index(char* currency);