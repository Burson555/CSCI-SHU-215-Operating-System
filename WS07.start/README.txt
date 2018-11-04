Bosen Yang by570


####################################################################################################
compiling rules START
####################################################################################################

make: running RR, MFQ and IORR algorithms on task files

make RR: Q1, running RR on 3 task files and produce 3 output files in folder opf
make MFQ: Q2, running MFQ on 3 task files and produce 3 output files in folder opf
make runIO: Q3, running IORR on task file tasks4 and produce an output file in folder opf

make clean: remove all files in bin/, src/ and lib/ for a fresh restart

####################################################################################################
compiling rules END
####################################################################################################



####################################################################################################
files description START
####################################################################################################

include/os-scheduling.h: header file for all questions

bin/sched-simulator: executable file for all questions

src/sched-policies.c: source file for all questions
src/sched-simulator.c: source file for all questions

opf/sched-runR1.txt: results of RR on tasks1
opf/sched-runR2.txt: results of RR on tasks2
opf/sched-runR3.txt: results of RR on tasks3

opf/sched-runM1.txt: results of MFQ on tasks1
opf/sched-runM2.txt: results of MFQ on tasks2
opf/sched-runM3.txt: results of MFQ on tasks3

opf/sched-runIO.txt: results of IORR on tasks4

./makefile: contains the compilation instructions all questions

####################################################################################################
files description END
####################################################################################################



####################################################################################################
Q1 COMMENTS START
####################################################################################################

Easy.

####################################################################################################
Q1 COMMENTS END
####################################################################################################



####################################################################################################
Q2 COMMENTS START
####################################################################################################

At first, I implemented the MFQ algorithm in a preemptive way.
Actually it turned out to be easier than the non-preemptive way, which the assignment asks us to do.
Because, when a new task is inserted into a higher priority queue and the current task hasn't finished its quantum, we have to skip the new task while calling scheduler function, searching for next task to process.
Such requirement takes extra work, which in my implementation needs a static variable (called currentQueue).

####################################################################################################
Q2 COMMENTS END
####################################################################################################



####################################################################################################
Q3 COMMENTS START
####################################################################################################

This question shouldn't be hard if we do it step by step:
	1. add IO related properties to data type task
	2. modify the sscanf function in sched-simulator.c so the program task IO data
	3. add an if under condition if (tasks[i].state == RUNNING), to start an IO
	4. add an external funtion which can find the correct task to run, because in our case, the first task in queue may not be runable if it's waiting for an IO
	5. also the function checks whether the task finishes its IO request
	6. consider the case when a task comes back from IO with tasks[i].executionTime == tasks[i].computationTime. Without this condition checking, such tasks can fall into infinite executions even if tasks[i].executionTime > tasks[i].computationTime
** the steps talked about above are important ones. I've omitted some easy and common-sense ones

####################################################################################################
Q3 COMMENTS END
####################################################################################################

