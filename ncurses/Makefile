.SUFFIXES: .cpp .o

FILES=main1 main2 main3 main4 main5 main6 main7 main8 main9 main10 main11

CXX=g++
OBJ_EXT=.o
EXE_EXT=.out

TARGETS=${FILES:%=%${EXE_EXT}}
OBJS=${FILES:%=%${OBJ_EXT}}

ifneq (${TPARTY_HOME}, "")
CXXFLAGS=-I${TPARTY_HOME}/local/include
LDFLAGS=-L${TPARTY_HOME}/local/lib
endif

CXXFLAGS+=-Wall -std=c++11
LDFLAGS+=-lpthread -lncurses

.PONEY=all clean

all: ${TARGETS}

clean:
	rm -rf ${OBJS} ${TARGETS}

%.out: %.o
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

