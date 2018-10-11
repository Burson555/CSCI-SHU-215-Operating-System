#include <os-scheduling.h>


int admitNewTasks(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j;
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
    return 1;
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
    printQueues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
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
    int i, j, minCompTime;
    
    // N.B: SJF does not require any queue!

    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running? Has that task finished its computations?
    //   If so, put it in terminated state
    //   If not, continue this task
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                break;
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    //Otherwise, find the task in READY state that has the shortest computation time
    j = -1;
    minCompTime = 0;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            if ((j == -1) || (minCompTime > tasks[i].computationTime)) {
                j = i;
                minCompTime = tasks[i].computationTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, remainingCompTime, minRemainingCompTime;
    
    // N.B: SRTF does not require any queue!
    
    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running?
    //      => determine its remaining computation time
    //   Has that task finished its computations?
    //      => put it in terminated state
    //   else
    //      => put it back to READY
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if (remainingCompTime == 0) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
            } else {
                tasks[i].state = READY;
            }
            break;
        }
    }
    
    //Now elect the task in READY state that has the shortest remaining time
    j = -1;
    minRemainingCompTime = -1;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if ((j == -1) ||
                (minRemainingCompTime > remainingCompTime)) {
                j = i;
                minRemainingCompTime = remainingCompTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}

int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}

int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}
