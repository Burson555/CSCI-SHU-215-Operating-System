CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: runstack# ${LIB}/libstack.a
#all: ${LIB}/libstack.a ${LIB}/libfifo.a


# ${LIB}/libstack.a: ${OBJ}/list_impl.o ${OBJ}/stack_list.o
	# ar rs ${LIB}/libstack.a ${OBJ}/list_impl.o ${OBJ}/stack_list.o

# ${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
	# ${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

# ${OBJ}/stack_list.o: ${INC}/list.h ${INC}/stack.h ${SRC}/stack_list.c
	# ${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}



#${LIB}/libstack.a: ${OBJ}/stack_array.o
#	ar rs ${LIB}/libstack0.a ${OBJ}/stack_array.o
#${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
#	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC} -DSTACK_SIZE=12



${LIB}/libfifo.a: ${OBJ}/list_impl.o ${OBJ}/fifo_list.o
	ar rs ${LIB}/libfifo.a ${OBJ}/list_impl.o ${OBJ}/fifo_list.o

${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

${OBJ}/fifo_list.o: ${INC}/list.h ${INC}/fifo.h ${SRC}/fifo_list.c
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}





# ${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
	# ${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}
${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}






#${BIN}/stack_test: ${OBJ}/stack_test.o ${OBJ}/stack_array.o
#	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${OBJ}/stack_array.o


# ${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a
	# ${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${LIB}/libstack.a
${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o ${LIB}/libfifo.a


runstack: ${BIN}/fifo_test
	echo "Testing Fifo"
	${BIN}/fifo_test

# runstack: ${BIN}/stack_test
	# echo "Testing Stack"
	# ${BIN}/stack_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*