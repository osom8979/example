.SUFFIXES: .cpp .o

FILES=main1 main2

CXX=g++
OBJ_EXT=.o
EXE_EXT=.out

TARGETS=${FILES:%=%${EXE_EXT}}
OBJS=${FILES:%=%${OBJ_EXT}}

ifneq (${TPARTY_HOME}, "")
CXXFLAGS=-I${TPARTY_HOME}/local/include
LDFLAGS=-L${TPARTY_HOME}/local/lib
ifneq ($(uname -s), "Darwin")
LDFLAGS=-Wl,-rpath,${TPARTY_HOME}/local/Library/Frameworks
endif
endif

CXXFLAGS+=-Wall -std=c++11
LDFLAGS+=-lpthread -lsfml-graphics -lsfml-window -lsfml-system

.PONEY=all clean

all: ${TARGETS}

clean:
	rm -rf ${OBJS} ${TARGETS}

%.out: %.o
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

