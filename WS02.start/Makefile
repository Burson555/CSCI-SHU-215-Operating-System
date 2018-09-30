CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

# ####################################################################################################
# Q2 START
# compile decoder from decoder.c
# ####################################################################################################
${BIN}/decoder: ${SRC}/decoder.c ${INC}/decoder.h
	${CC} -o ${BIN}/decoder ${SRC}/decoder.c ${INC}/decoder.h -I${INC}

Q2: ${BIN}/decoder
	@echo "\nTesting Decoder\n\n"
	${BIN}/decoder
# ####################################################################################################
# Q2 END
# ####################################################################################################


# ####################################################################################################
# Q3 START
# compile converters from converters.c
# ####################################################################################################
${BIN}/converters: ${SRC}/converters.c ${INC}/converters.h
	${CC} -o ${BIN}/converters ${SRC}/converters.c ${INC}/converters.h -I${INC}

Q3: ${BIN}/converters
	@echo "\nTesting Converter\n\n"
	${BIN}/converters CNY "100.0"
#	${BIN}/converters USD "100.0"
#	${BIN}/converters JPY "100.0"
#	${BIN}/converters EUR "100.0"
#	${BIN}/converters GBP "100.0"
# ####################################################################################################
# Q3 END
# ####################################################################################################

	
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
