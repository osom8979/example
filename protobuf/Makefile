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
endif

CXXFLAGS+=-Wall -std=c++11
LDFLAGS+=-lpthread -lprotobuf

PROTO_OBJS=test.pb.cc test.pb.h test.o

.PONEY=all clean

all: ${TARGETS}

clean:
	rm -rf ${OBJS} ${TARGETS} ${PROTO_OBJS}

%.out: %.o test.o
	${CXX} ${LDFLAGS} -o $@ $^

test.o: test.pb.cc
	${CXX} ${CXXFLAGS} -o $@ -c $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

test.pb.cc: test.proto
	protoc --cpp_out=. $^

