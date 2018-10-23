#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_SIZE 512
#define MAX_NB_OF_TASKS 32
#define MAX_TASK_NAME_SIZE 32
#define MAX_NB_OF_QUEUES 8
#define NB_OF_QUEUES 3

#define UPCOMING 0
#define READY 1
#define RUNNING 2
#define SLEEPING 3
#define TERMINATED 4

typedef struct task {
    /* Initial elements */
    char name[MAX_TASK_NAME_SIZE]; //task name
    unsigned int computationTime; //task duration
    unsigned int arrivalDate; //date of insertion in the system
    /* Used by scheduler */
    unsigned int state;
    unsigned int executionTime; //nb of cycles on processor
    unsigned int turnaroundTime; //turnaround time for each task
    unsigned int currentQuantum; //current quantum for each task
    
    unsigned int duratioinIO; //how long an IO request takes
    unsigned int frequencyIO; //how often to make an IO
    unsigned int timeIOStart; //time the task starts IO request
    unsigned int timeToRequest; //time remaining to make IO request
} task;

typedef struct sched_data {
    int nbOfQueues;
    int queues[MAX_NB_OF_QUEUES][MAX_NB_OF_TASKS];
    int prioOfQueues[MAX_NB_OF_QUEUES];
} sched_data;

int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime);

int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum);

int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum);

int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum);