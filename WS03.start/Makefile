CFLAGS=-Iinclude -Wall

all: build RR MFQ runIO

RR: runR1 runR2 runR3

MFQ: runM1 runM2 runM3

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	mkdir opf
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

runR1:
	bin/sched-simulator tsk/tasks1 RR 4 > opf/sched-runR1.txt
	cat  opf/sched-runR1.txt

runR2:
	bin/sched-simulator tsk/tasks2 RR 4 > opf/sched-runR2.txt
	cat  opf/sched-runR2.txt

runR3:
	bin/sched-simulator tsk/tasks3 RR 4 > opf/sched-runR3.txt
	cat  opf/sched-runR3.txt

runM1:
	bin/sched-simulator tsk/tasks1 MFQ 3 > opf/sched-runM1.txt
	cat  opf/sched-runM1.txt

runM2:
	bin/sched-simulator tsk/tasks2 MFQ 3 > opf/sched-runM2.txt
	cat  opf/sched-runM2.txt

runM3:
	bin/sched-simulator tsk/tasks3 MFQ 3 > opf/sched-runM3.txt
	cat  opf/sched-runM3.txt

runIO:
	bin/sched-simulator tsk/tasks4 IORR 4 > opf/sched-runIO.txt
	cat  opf/sched-runIO.txt

clean:
	rm -rf bin obj opf