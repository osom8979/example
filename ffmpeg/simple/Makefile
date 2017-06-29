.SUFFIXES: .cpp .o

CXX=g++
TARGET=a.out
OBJS=main.o
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavformat -lavutil -lavfilter -lavdevice -lswscale

.PONEY=all clean

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	${CXX} -o $@ ${LDFLAGS} $^

