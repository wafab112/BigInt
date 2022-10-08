CXX := g++
FLAGS := -std=c++11

WDIR := ./BigInt
BDIR := ./build

HEADERS := $(addprefix ${WDIR}/,UBigInt.h types.h array_utils.h)

SOURCE_NAMES := arithmetics comparisons constructors extras
OBJS := $(addprefix ${BDIR}/,$(addsuffix .o, ${SOURCE_NAMES}))

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

TEST_PATH := ./tests
TESTS = test_constructors.cpp

${TEST_PATH}/bigint.a: all
	cp ${TARGET} ${TEST_PATH}
	cp ${BDIR}/include/* ${TEST_PATH}

${TEST_PATH}/run: ${TEST_PATH}/main_test.cpp $(addprefix ${TEST_PATH}/,${TESTS})
	${CXX} ${FLAGS} -o $@ $< ${TEST_PATH}/bigint.a

test: ${TEST_PATH}/run
	${TEST_PATH}/run

clean:
	rm -r build/*
