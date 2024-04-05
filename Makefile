CXX := g++
FLAGS := -std=c++11

WDIR := ./BigInt
BDIR := ./build

HEADERS := $(addprefix ${WDIR}/,UBigInt.h types.h array_utils.h)

SOURCE_NAMES := arithmetics comparisons constructors extras
OBJS := $(addprefix ${BDIR}/,$(addsuffix .o, ${SOURCE_NAMES}))

TARGET = ${BDIR}/lib_bigint.a

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

TEST_PATH := ./tests
TEST_SRC := ${TEST_PATH}/src
TEST_INCLUDES := ${TEST_SRC}/include
TEST_LIBS := ${TEST_PATH}/lib
TEST_TARGET := ${TEST_PATH}/run

TESTS = test_constructors.cpp

${TEST_LIBS}/lib_bigint.a: all
	mkdir -p ${TEST_LIBS}
	cp ${TARGET} ${TEST_LIBS}
	mkdir -p ${TEST_INCLUDES}
	cp ${BDIR}/include/* ${TEST_INCLUDES}
	cp doctest.h ${TEST_INCLUDES}

${TEST_TARGET}: ${TEST_SRC}/main_test.cpp $(addprefix ${TEST_SRC}/,${TESTS}) ${TEST_LIBS}/lib_bigint.a
	${CXX} ${FLAGS} -o $@ $< ${TEST_LIBS}/lib_bigint.a

test: ${TEST_TARGET}
	${TEST_TARGET}

clean:
	rm -r build/*
	rm ${TEST_TARGET}
	rm -r ${TEST_LIBS}
	rm -r ${TEST_INCLUDES}
