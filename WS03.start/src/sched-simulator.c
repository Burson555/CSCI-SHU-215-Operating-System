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
int scheduler(task tasks[], int nbOfTasks, sched_data *schedData, int currentTime) {
//    return FCFS(tasks, nbOfTasks, schedData, currentTime);
//    return SJF(tasks, nbOfTasks, schedData, currentTime);
    return SRTF(tasks, nbOfTasks, schedData, currentTime);
}


int main(int argc, char *argv[]){
    char line [MAX_LINE_SIZE]; /* or other suitable maximum line size */
    task tasks[MAX_NB_OF_TASKS];
    sched_data *schedData = (sched_data*)malloc(sizeof(sched_data));
    int nbOfTasks = 0;
    int time = 0;
    int taskIndex;
    
    /**** Read the task file, and store into a struct ****/
    FILE *file = fopen (argv[1], "r" );
    if (file == NULL) {
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
    printf("Scheduling the set of tasks\n\n");
    
    
    while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
        printTasks(tasks, nbOfTasks);
        taskIndex = scheduler(tasks, nbOfTasks, schedData, time);
        if (taskIndex >= 0) {
            printf("\nTime %d: %s\n", time,  tasks[taskIndex].name);
        } else {
            printf("\nTime %d: no task to schedule\n", time);
        }
        time ++;
    }
    
    
    /**** That's all folks ****/
    printTasks(tasks, nbOfTasks);
    time --;
    printf("All done after %d units of time\n", time);
    
    /**** Print the statistics ****/
    printf("\nTask\tturnaroundTime\tpenaltyRate\n");
    int k;
    double waitingTime;
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


