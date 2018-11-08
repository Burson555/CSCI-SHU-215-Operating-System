// do I need extra data structures?
// leave the error detection to the last step

// FIRST parse every input argument and store them into local variables
// i.e.,    int server_id
//          char* client_id
//          int currency
//          float amount

// NOTE
//          1. client_id must be string so that server can locate the shm by name /<client_id>_shm:0
//          2. use another shared memory to store information like client_id

// SECOND create shared memory

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


int main(int argc, char *argv[]){

    char line [MAX_LINE_SIZE]; /* or other suitable maximum line size */

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
        sscanf(line, "%s %u %u %u %u\n", tasks[nbOfTasks].name, 
        	&(tasks[nbOfTasks].computationTime), &(tasks[nbOfTasks].arrivalDate), 
        	&(tasks[nbOfTasks].duratioinIO), &(tasks[nbOfTasks].frequencyIO));
        tasks[nbOfTasks].state = UPCOMING;
        tasks[nbOfTasks].executionTime = 0;
        // printf("T%d - duratioinIO:%u; frequencyIO:%u.\n", nbOfTasks + 1, 
        // 	tasks[nbOfTasks].duratioinIO, tasks[nbOfTasks].frequencyIO);
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


