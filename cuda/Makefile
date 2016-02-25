.SUFFIXES: .cu .o

NVCC=nvcc
TARGET=a.out
OBJS=main.o
CXXFLAGS=
LDFLAGS=

.PONEY=all clean

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	${NVCC} -o $@ ${LDFLAGS} $^

%.o: %.cu
	${NVCC} -c -o $@ ${CXXFLAGS} $^

