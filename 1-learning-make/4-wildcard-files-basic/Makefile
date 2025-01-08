###############
## VARIABLES ##
###############
# Note: Variables can only be strings
# Note: Single or double quotes for variable names or values have no meaning to Make

# CC: Program for compiling C programs; default cc
# CXX: Program for compiling C++ programs; default g++
# CFLAGS: Extra flags to give to the C compiler
# CXXFLAGS: Extra flags to give to the C++ compiler
# CPPFLAGS: Extra flags to give to the C preprocessor
# LDFLAGS: Extra flags to give to the linker

DEBUG = 1
EXECUTABLE_NAME = main

CXX_STANDARD = c++17
CXX_WARNINGS = -Wall -Wextra -Wpedantic
CXX = g++
CXXFLAGS = ${CXX_WARNINGS} -std=${CXX_STANDARD}
LDFLAGS = 

ifeq (${DEBUG}, 1) 
	CXXFLAGS += -g -O0
else
	CXXFLAGS += -O3
endif

COMPILER_CALL = ${CXX} ${CXXFLAGS}

CXX_OBJECTS = my_lib.o main.o

build: ${CXX_OBJECTS}
	${COMPILER_CALL} ${CXX_OBJECTS} -o ${EXECUTABLE_NAME}

run:
	./${EXECUTABLE_NAME}


clean:
	rm -f *.o
	rm -f ${EXECUTABLE_NAME}


##############
## PATTERNS ##
##############
# $@: the file name of the target
# $<: the name of the first ependency
# $^: the names of all prerequisites
%.o: %.cc
	${COMPILER_CALL} -c $< -o $@
