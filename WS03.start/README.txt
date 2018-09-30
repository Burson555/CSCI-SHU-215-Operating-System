Bosen Yang by570




Detailed List of Files
/include/*
	stack.h
	list.h
	fifo.h

/lib/*
	libstack0.a		Question 2
	libstack.a		Question 5
	libfifo.a		Question 6

/src/*
	stack_test.c		Question 3 5
	stack_list.c		Question 5
	stack_array.c		Question 2 3
	list_impl.c		Question 4 5 6
	fifo_test.c		Question 6
	fifo_list.c		Question 6





Explanation of compilation rules && Answer for Question 7
Question 1
	No compilation rule written for this question.
Question 2
	${LIB}/libstack.a: ${OBJ}/stack_array.o
		ar rs ${LIB}/libstack.a ${OBJ}/stack_array.o
	The rule above archives object file obj/stack_array.o by creating a static library called libstack.a

Question 3
	${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
		${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC} -DSTACK_SIZE=3
	I added a term "-DSTACK_NAME=3" to the end of the original code, changing the value of a variable/macro called STACK_SIZE to 3.
Question 4
	No compilation rule written for this question.
Question 5
	${LIB}/libstack.a: ${OBJ}/list_impl.o ${OBJ}/stack_list.o
		ar rs ${LIB}/libstack.a ${OBJ}/list_impl.o ${OBJ}/stack_list.o
	${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
		${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}
	${OBJ}/stack_list.o: ${INC}/list.h ${INC}/stack.h ${SRC}/stack_list.c
		${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}
	...(unimportant codes that compiles stack_test.c)
	${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a
		${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${LIB}/libstack.a

	The second and third lines of code compile the source files into object files.
	The first line of code archives the object files in a library called libstack.a
	The last line of code creates an executable file with the stack_test.o object file and the staic library we just created.

Question 6
	${LIB}/libfifo.a: ${OBJ}/list_impl.o ${OBJ}/fifo_list.o
		ar rs ${LIB}/libfifo.a ${OBJ}/list_impl.o ${OBJ}/fifo_list.o
	This line archives the object files into a static library called libfifo.a

	${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
		${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}
	This line creates an object file from source file list_impl.c

	${OBJ}/fifo_list.o: ${INC}/list.h ${INC}/fifo.h ${SRC}/fifo_list.c
		${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}
	This line compiles source file fifo_list.c to derive object file fifo_list.o

	${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
		${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}
	This line compiles source file fifo_test.c to derive object file fifo_test.o

	${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
		${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	This line links fifo_test.o object file and the library we just created to derive an executable file fifo_test

Question 7
	I would make the change in file list.h by adding a new integer variable into struct list_type.
	This variable will be initialized as 0.
	When inserting a new element into the list, the value of this variable increases by 1.
	When extracting an element from the list, the value of this variable decreases by 1.
	The value of this variable will always be equal to the number of elements in the list.
	If we want to know the size of the list, we can just return this variable value, and it takes only O(1).




Comments
Question 1
	While trying to compile the program by calling make, Terminal tells me there's no makefile file found.
	I know the file called makefile.ini is exactly what I need.
	Then I tried entering make makefile.ini, but failed too.
	I finally succeeded by calling make after changing file name of makefile.ini into makefile using mv.
Question 2
	This one actually a bit tricky.
	I did wrote a line "${LIB}/libstack.a: ${OBJ}/stack_array.o\n\tar rs ${LIB}/libstack.a ${OBJ}/stack_array.o" in Makefile.
	There's no problem with this code, but the archiving just doesn't happen.
	Finally I found it's because I didn't include ${LIB}/libstack.a as a prerequisite under target all.
Question 3
	No comments. This one is easy if you know how to use -D.
	The tricky point is that we should delete the line "#define STACK_SIZE 12" in stack_array.c.
	Or variable STACK_SIZE would be redefine back to 12 while compiling.
Question 4
	A refresher on Data Structure, which one shouldn't have trouble of.
Question 5
	While compiling source file and header file into object file, I forgot to include -I${INC} in the instruction.
	It turned out that the program can't find the header file.	
	Everything's coming back to normal after I added -I${INC}.
Question 6
	Easy. No comments.


