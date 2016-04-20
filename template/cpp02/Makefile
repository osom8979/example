#.SUFFIXES: .cpp .o

CXX=g++
EXE_EXT=.out
OBJ_EXT=.o
DEBUG=-g
TARGET=a${EXE_EXT}
FILES=main test01
OBJS=${FILES:%=%${OBJ_EXT}}
LIBS=pthread

ifneq (${TPARTY_HOME}, "")
CXXFLAGS=-I${TPARTY_HOME}/local/include
LDFLAGS=-L${TPARTY_HOME}/local/lib
endif

CXXFLAGS+=-Wall -std=c++11
LDFLAGS+=${LIBS:%=-l%}

DEBUG_TARGET=${TARGET:${EXE_EXT}=${DEBUG}${EXE_EXT}}
DEBUG_OBJS=${OBJS:${OBJ_EXT}=${DEBUG}${OBJ_EXT}}

.PONEY=all clean release debug

all: release debug

release: ${TARGET}

debug: ${DEBUG_TARGET}

${TARGET}: ${OBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^

${DEBUG_TARGET}: ${DEBUG_OBJS}
	${CXX} ${CXXFLAGS} ${DEBUG} ${LDFLAGS} -o $@ $^

clean:
	rm -rf ${TARGET} ${DEBUG_TARGET} ${OBJS} ${DEBUG_OBJS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

%-g.o: %.cpp
	${CXX} ${CXXFLAGS} ${DEBUG} -o $@ -c $^
 
