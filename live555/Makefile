.SUFFIXES: .cpp .o

CXX=g++
TARGET=a.out
OBJS=main.o
CXXFLAGS=-Wall -std=c++11 -Iinclude \
		 -Iinclude/BasicUsageEnvironment \
		 -Iinclude/UsageEnvironment \
		 -Iinclude/groupsock \
		 -Iinclude/liveMedia
LDFLAGS=-llive555

.PONEY=all clean

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	${CXX} -o $@ ${LDFLAGS} $^

