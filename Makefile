CXX = g++
FLAGS = -std=c++11

WDIR = ./BigInt
BDIR = ./build

HEADERS = ${WDIR}/UBigInt.h ${WDIR}/types.h ${WDIR}/array_utils.h
OBJS = ${BDIR}/arithmetics.o ${BDIR}/comparisons.o ${BDIR}/constructors.o ${BDIR}/extras.o

TARGET = ${BDIR}/bigint.a

all: ${TARGET}
	mkdir -p ${BDIR}/include
	cp ${WDIR}/*.h ${BDIR}/include

${TARGET}: ${OBJS}
	ar rcs $@ $^
	ranlib $@

${BDIR}/arithmetics.o: ${WDIR}/arithmetics.cpp ${HEADERS}
	${CXX} ${FLAGS} -c $< -o $@

${BDIR}/comparisons.o: ${WDIR}/comparisons.cpp ${HEADERS}
	${CXX} ${FLAGS} -c $< -o $@

${BDIR}/constructors.o: ${WDIR}/constructors.cpp ${HEADERS}
	${CXX} ${FLAGS} -c $< -o $@

${BDIR}/extras.o: ${WDIR}/extras.cpp ${HEADERS}
	${CXX} ${FLAGS} -c $< -o $@

clean:
	rm -r build/*
