/**************************************/
/* (c) L. Apvrille, Telecom ParisTech */
/**************************************/


#include <os-scheduling.h>


char * states[] = {"upcoming  ", "ready     ", "running   ", "suspended ", "terminated"};


char * getStateString(int state) {
    return states[state];
}

/* Returns the number of tasks that still have to be run, */
/* that is, all tasks which still have computations to perform */
int hasTasksToSchedule(task tasks[], int nbOfTasks) {
    int total = 0;
    int i;
    
    for(i=0; i<nbOfTasks; i++) {
        if (tasks[i].state != TERMINATED) {
            total ++;
        }
    }
    return total;
}


void printTasks(task tasks[], int nbOfTasks) {
    int i;
    
    for(i=0; i<nbOfTasks; i++) {
        printf("Task: %s \t arrivalDate:%d    \t state:%s \t computations:%d/%d\n",
               tasks[i].name, tasks[i].arrivalDate, getStateString(tasks[i].state),
               tasks[i].executionTime, tasks[i].computationTime);
    }
}


/* Returns the index of the elected task  */
/*         -1 if no task could be elected */
int schedulerIORR(task tasks[], int nbOfTasks, sched_data *schedData, int currentTime, int quantum) {
    return IORR(tasks, nbOfTasks, schedData, currentTime, quantum);
}
int schedulerRR(task tasks[], int nbOfTasks, sched_data *schedData, int currentTime, int quantum) {
    return RR(tasks, nbOfTasks, schedData, currentTime, quantum);
}
int schedulerMFQ(task tasks[], int nbOfTasks, sched_data *schedData, int currentTime, int quantum) {
    return MFQ(tasks, nbOfTasks, schedData, currentTime, quantum);
}
int scheduler(task tasks[], int nbOfTasks, sched_data *schedData, int currentTime) {
    return FCFS(tasks, nbOfTasks, schedData, currentTime);
}

int main(int argc, char *argv[]){
    char line [MAX_LINE_SIZE]; /* or other suitable maximum line size */
    task tasks[MAX_NB_OF_TASKS];
    sched_data *schedData = (sched_data*)malloc(sizeof(sched_data));
    int nbOfTasks = 0;
    int time = 0;
    int taskIndex;
    /********************************/
    int quantum;
    int nbPolicy;

    /**** Define Quantum ****/
    quantum = atoi(argv[3]);
    printf("%s%d\n\n", "The Quantum is ", quantum);

    /**** Determin Scheduling Policy ****/
    // printf("%s\n\n", argv[2]);
    if (strncmp(argv[2], "RR", 2) == 0){
        nbPolicy = 0;}
    else if (strncmp(argv[2], "MFQ", 3) == 0){
        nbPolicy = 1;}
    else if (strncmp(argv[2], "IORR", 4) == 0){
        nbPolicy = 2;}
    // Neither RR or MFQ are entered
    else{
        nbPolicy = 3;}
    // printf("%s%d\n\n", "# of Policy ", nbPolicy);

    /**** Read the task file, and store into a struct ****/
    FILE *file = fopen (argv[1], "r" );
    if (file == NULL) {
        printf("%s\n", "Noooooo");
        perror(argv[1]);
        return -1;
    }
    
    /* Read the file line by line */
    printf("Loading file of tasks\n");
    while (fgets(line, sizeof(line), file) != NULL ) {
        sscanf(line, "%s %u %u\n", tasks[nbOfTasks].name, &(tasks[nbOfTasks].computationTime), &(tasks[nbOfTasks].arrivalDate));
        tasks[nbOfTasks].state = UPCOMING;
        tasks[nbOfTasks].executionTime = 0;
        nbOfTasks ++;
    }
    fclose(file);
    printf("%d tasks loaded\n\n", nbOfTasks);
    
    /**** Schedule the set of tasks ****/
    printf("Scheduling the set of tasks\n");
    
    switch (nbPolicy){
        case 0:
            printf("%s\n\n", "Using Scheduling Policy Round Robin");
            while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
                printTasks(tasks, nbOfTasks);
                taskIndex = schedulerRR(tasks, nbOfTasks, schedData, time, quantum);
                if (taskIndex >= 0) {
                    printf("Time %d: %s\n\n", time,  tasks[taskIndex].name);
                } else {
                    printf("Time %d: no task to schedule\n\n", time);
                }
                time ++;
            }
            break;
        case 1:
            printf("%s\n\n", "Using Scheduling Policy Multilevel Feedback Queue");
            while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
                printTasks(tasks, nbOfTasks);
                taskIndex = schedulerMFQ(tasks, nbOfTasks, schedData, time, quantum);
                if (taskIndex >= 0) {
                    printf("Time %d: %s\n\n", time,  tasks[taskIndex].name);
                } else {
                    printf("Time %d: no task to schedule\n\n", time);
                }
                time ++;
            }
            break;
        case 2:
            printf("%s\n\n", "Using Scheduling Policy Input/Output Round Robin");
            while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
                printTasks(tasks, nbOfTasks);
                taskIndex = schedulerIORR(tasks, nbOfTasks, schedData, time, quantum);
                if (taskIndex >= 0) {
                    printf("Time %d: %s\n\n", time,  tasks[taskIndex].name);
                } else {
                    printf("Time %d: no task to schedule\n\n", time);
                }
                time ++;
            }
            break;
        default:
            printf("%s\n\n", "Using Scheduling Policy First Come First Serve");
            while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
                printTasks(tasks, nbOfTasks);
                taskIndex = scheduler(tasks, nbOfTasks, schedData, time);
                if (taskIndex >= 0) {
                    printf("Time %d: %s\n\n", time,  tasks[taskIndex].name);
                } else {
                    printf("Time %d: no task to schedule\n\n", time);
                }
                time ++;
            }
            break;
    }
    
    
    
    /**** That's all folks ****/
    printTasks(tasks, nbOfTasks);
    time --;
    printf("\nAll done after %d units of time\n", time);
    
    /**** Print the statistics ****/
    printf("\nTask\tturnaroundTime\tpenaltyRate\n");
    int k;
    double waitingTime = 0;
    for (k = 0; k < nbOfTasks; k++){
        printf("%s\t", tasks[k].name);
        printf("%d\t\t", tasks[k].turnaroundTime);
        printf("%.2f\t\t", (double)(tasks[k].turnaroundTime)/(double)tasks[k].computationTime);
        printf("\n");
        waitingTime += (tasks[k].turnaroundTime - tasks[k].computationTime);
    }
    printf("\nAverage waiting time: %.2f\n\n", waitingTime/nbOfTasks);
    
    return 0;
}


