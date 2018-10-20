CFLAGS=-Iinclude -Wall

all: build RR

RR: runR1 runR2 runR3

MFQ: runM1 runM2 runM3

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

runR1:
	bin/sched-simulator "tasks1" RR 4 >sched-runR1.txt
	cat sched-run1.txt

runR2:
	bin/sched-simulator "tasks2" RR 4 >sched-runR2.txt
	cat sched-run2.txt

runR3:
	bin/sched-simulator "tasks3" RR 4 >sched-runR3.txt
	cat sched-run3.txt

runM1:
	bin/sched-simulator "tasks1" MFQ 3 >sched-runM1.txt
	cat sched-run1.txt

runM2:
	bin/sched-simulator "tasks2" MFQ 3 >sched-runM2.txt
	cat sched-run2.txt

runM3:
	bin/sched-simulator "tasks3" MFQ 3 >sched-runM3.txt
	cat sched-run3.txt

clean:
	rm -rf bin obj sched-run.txt