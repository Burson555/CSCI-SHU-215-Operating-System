#include <os-scheduling.h>


void print_queues(task tasks[], sched_data* schedData) {
    int i, j, taskIndex = 0;
    printf("Nb of queues %d\n", schedData->nbOfQueues);
    for (i = 0; i < schedData->nbOfQueues; i++) {
        j = 0;
        printf("Q%d => ", i);
        while (j < MAX_NB_OF_TASKS) {
            taskIndex = schedData->queues[i][j];
            if (taskIndex == -1) {
                j = MAX_NB_OF_TASKS;
            } else {
                printf("%s ", tasks[taskIndex].name);
                j++;
            }
        }
        printf("\n");
    }
}


int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i, j;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    
    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            j++;
        }
    }
    print_queues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                // **************************************
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}



int SJF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i, j;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    
    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            j++;
        }
    }
    print_queues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                int shortestTask = 0;
                // find the first READY task
                for (j = 1; j < MAX_NB_OF_TASKS; j++) {
                    if (schedData->queues[0][j] == -1){
                        j = MAX_NB_OF_TASKS;
                    }
                    else if (tasks[schedData->queues[0][j]].state == READY){
                        shortestTask = j;
                        j = MAX_NB_OF_TASKS;
                    }
                }
                // find the shortest task
                for (j = shortestTask + 1; j < MAX_NB_OF_TASKS; j++){
                    if (schedData->queues[0][j] == -1){
                        j = MAX_NB_OF_TASKS;
                    }
                    else if (tasks[schedData->queues[0][j]].state == READY && tasks[schedData->queues[0][j]].computationTime < tasks[schedData->queues[0][shortestTask]].computationTime){
                        shortestTask = j;
                    }
                }
                // find it or not? Both possible
                schedData->queues[0][0] = schedData->queues[0][shortestTask];
                // we have to remove the first task from queque
                for (j = shortestTask; j<MAX_NB_OF_TASKS - 1; j++){
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
                
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}



int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i, j;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    
    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            if (j == 0){
                schedData->queues[0][j] = i;
                j++;
                continue;
            }
            // the coming task is shorter than all in the queue
            if (tasks[i].computationTime < tasks[schedData->queues[0][0]].computationTime - tasks[schedData->queues[0][0]].executionTime){
                int k;
                for (k = j; k > 0; k--){
                    schedData->queues[0][k] = schedData->queues[0][k - 1];
                }
                schedData->queues[0][0] = i;
            } else { // the coming task is not the shortest
                int k;
                // find the correct insertion position
                for (k = 1; k < nbOfTasks; k++){
                    if (schedData->queues[0][k] == -1){
                        break;
                    }
                    if (tasks[i].computationTime < tasks[schedData->queues[0][k]].computationTime - tasks[schedData->queues[0][k]].executionTime){
                        break;
                    }
                }
                // now schedData->queues[0][k] = -1 || realTask
                if (schedData->queues[0][k] != -1){
                    int t;
                    for (t = j; t > k; t--){
                        schedData->queues[0][t] = schedData->queues[0][t - 1];
                    }
                }
                schedData->queues[0][k] = i;
            }
            j++;
        }
    }
    print_queues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                // **************************************
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j + 1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}
