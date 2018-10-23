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
    printf("i = %d\n", i + 1);
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



int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {
    
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
            tasks[nbOfTasks].currentQuantum = 0;
            j++;
        }
    }
    print_queues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, CHECK THE QUANTUM
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* still have quantum */
                if (tasks[i].currentQuantum < quantum){
                    tasks[i].executionTime ++;
                    tasks[i].currentQuantum ++;
                    return i;
                }
                /* the ELSE case, no quantum left */
                /* elect another task */
                /* we have 2 cases: current is OR isn't the last task in queue */
                /* both cases can be done with following codes */
                else{
                    tasks[i].state = READY;
                    tasks[i].currentQuantum = 0;
                    j = 0;
                    while (schedData->queues[0][j] != -1){
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                        j++;
                    }
                    schedData->queues[0][j-1] = i;
                }
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].currentQuantum ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}



int currentQueue;
int elect_new_task(sched_data* schedData) {
    currentQueue = 0;
    int i = schedData->queues[currentQueue][0];
    while(currentQueue < NB_OF_QUEUES){
        if (schedData->queues[currentQueue][0] != -1){
            i = schedData->queues[currentQueue][0];
            break;
        }
        currentQueue++;
    }
    return i;
}

int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {
    
    int i, j;
    
    // Initialize multi queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = NB_OF_QUEUES;
        for (j = 0; j < NB_OF_QUEUES; j++){
            for (i = 0; i < MAX_NB_OF_TASKS; i++) {
                schedData->queues[j][i] = -1;
            }
        }
        // assign priorities
        for (j = 0; j < NB_OF_QUEUES; j++){
            schedData->prioOfQueues[j] = j+1;
        }
    }
    
    // Admit new tasks (currentTime >= arrivalTime)
    // // the preemptive case
    // currentQueue = 0;
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            tasks[i].currentQuantum = schedData->prioOfQueues[0]*quantum;
            printf("T%d%s%d\n", i + 1, " has quantum of ", tasks[i].currentQuantum);
            // // the preemptive case
            // // elect the coming task
            // // AND change the state of the current running task
            // if (j == 0){
            //     tasks[i].state = RUNNING;
            //     for (currentQueue = 1; currentQueue < NB_OF_QUEUES; currentQueue++){
            //         if (schedData->queues[currentQueue][0] != -1){
            //             tasks[schedData->queues[currentQueue][0]].state = READY;
            //             break;
            //         }
            //     }
            // }
            j++;
        }
    }
    print_queues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, CHECK THE QUANTUM

    /* i = elect(schedData);*/
    /* the code wbove will locate the element with the highest prio */
    // we modify it so that ti finds the RUNNING task
    i = schedData->queues[currentQueue][0];
    printf("current queue: %d\n", currentQueue);
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[currentQueue][j] = schedData->queues[currentQueue][j+1];
                }
            } else {
            	// NOT end the process
                /* still have quantum */
                if (tasks[i].currentQuantum != 0){
                    tasks[i].executionTime ++;
                    tasks[i].currentQuantum --;
                    return i;
                }
                /* the ELSE case, no quantum left */
                /* elect another task */
                /* we have 2 cases: current is OR isn't the last task in queue */
                /* both cases can be done with following codes */
                else{
                	// move to another queue
                	// traverse the whole queues to make election
                	j = 0;
                    while (schedData->queues[currentQueue][j] != -1){
                        schedData->queues[currentQueue][j] = schedData->queues[currentQueue][j+1];
                        j++;
                    }
                    if (currentQueue == NB_OF_QUEUES - 1)
                    	currentQueue = 0;
                    else
                    	currentQueue ++;
                    tasks[i].state = READY;
	                tasks[i].currentQuantum = schedData->prioOfQueues[currentQueue]*quantum;
	                printf("T%d%s%d\n", i + 1, " has quantum of ", tasks[i].currentQuantum);
	                // now we're staying at the current queue
                    j = 0;
                    // finding the correct place to insert
                    for (; j < MAX_NB_OF_TASKS; j++){
                        if (schedData->queues[currentQueue][j] == -1){
                            schedData->queues[currentQueue][j] = i;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = elect_new_task(schedData);
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].currentQuantum --;
        tasks[i].state = RUNNING;
        return i;
    } 
    else
    	currentQueue = 0;
    // we need the ELSE case because under this condition currentQueue is 3
    // we run into error at line 205
    
    // No task could be elected
    return -1;
}

int currentTask = -1;
int find_starting_position(task tasks[], sched_data* schedData, int currentTime) {
	// mission 1: check the I/O condition
	int i;
	int j = 0;
	for (; j < MAX_NB_OF_TASKS; j++){
		i = schedData->queues[0][j];
		if (i == -1)
			break;
		if (tasks[i].duratioinIO <= (currentTime - tasks[i].timeIOStart))
			if (tasks[i].timeToRequest > 0)
				tasks[i].state = READY;
	}
	// Mission 2: find the right task to return
	if (currentTask >= 0)
		return currentTask;
	for (j = 0; j < MAX_NB_OF_TASKS; j++){
		i = schedData->queues[0][j];
		if (tasks[i].state == READY){
			currentTask = i;
			return i;
		}
	}
	return -1;
}

int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {
    
    int i, j;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        printf("Quantum: %d\n", quantum);
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
        	printf("\ti = %d\n", i);
    		printf("\tfrequency: %d\n", tasks[i].frequencyIO);
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            tasks[nbOfTasks].currentQuantum = quantum;
            tasks[nbOfTasks].timeToRequest = tasks[nbOfTasks].frequencyIO;
            j++;
        }
    }
    print_queues(tasks, schedData);
    /*******************************************************************/

    // need codes here
    // this block checks the IO status
    // shoud do well with those non-IO tasks
    // should end with: i = schedData->queues[0][j];
    // where j is the index of the first non-SLEEPING element

    // need more properties about I/O
    // like timeIOStart (the time it sleeps)
    // 		&& timeToRequest

    /*******************************************************************/

    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, CHECK THE QUANTUM
    // i = schedData->queues[0][0];
    printf("i = %d\n", i);
    printf("currentQuantum: %d\n", tasks[i].currentQuantum);
    printf("timeToRequest: %d\n", tasks[i].timeToRequest);
    i = find_starting_position(tasks, schedData, currentTime);
    printf("i = %d\n", i);
    printf("currentQuantum: %d\n", tasks[i].currentQuantum);
    printf("timeToRequest: %d\n", tasks[i].timeToRequest);
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
        	if (tasks[nbOfTasks].timeToRequest == 0){
            	// the IO case
            	tasks[i].state = SLEEPING;
            	tasks[i].currentQuantum = quantum;
            	tasks[i].timeIOStart = currentTime;
            	tasks[nbOfTasks].timeToRequest = tasks[nbOfTasks].frequencyIO;
            	j = 0;
                while (schedData->queues[0][j] != -1){
                    schedData->queues[0][j] = schedData->queues[0][j + 1];
                    j++;
                }
                schedData->queues[0][j-1] = i;
                currentTask = -1;
            }
            else if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].turnaroundTime = currentTime - tasks[i].arrivalDate;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j + 1];
                }
                currentTask = -1;
            }
            else {
                /* still have quantum */
                if (tasks[i].currentQuantum != 0){
                    tasks[i].executionTime ++;
                    tasks[i].currentQuantum --;
                    tasks[nbOfTasks].timeToRequest --;
                    return i;
                }
                /* the ELSE case, no quantum left */
                /* elect another task */
                /* we have 2 cases: current is OR isn't the last task in queue */
                /* both cases can be done with following codes */
                else{
                    tasks[i].state = READY;
                    tasks[i].currentQuantum = quantum;
                    j = 0;
                    while (schedData->queues[0][j] != -1){
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                        j++;
                    }
                    schedData->queues[0][j-1] = i;
                    currentTask = -1;
                }
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    // i = schedData->queues[0][0];
    printf("the above code is never run\n");
    i = find_starting_position(tasks, schedData, currentTime);
    printf("i = %d\n", i);
    printf("currentQuantum: %d\n", tasks[i].currentQuantum);
    printf("timeToRequest: %d\n", tasks[i].timeToRequest);
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].currentQuantum --;
        tasks[nbOfTasks].timeToRequest --;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}
