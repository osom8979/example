
OPENCV_LIBS=-lopencv_core249.dll -lopencv_highgui249.dll
FFTW_LIBS=-lfftw3

CXXFLAGS=--std=c++11

PROJS = main main2 main3 main4

all: $(PROJS)

clean:
	rm -rf *.o $(PROJS)

main: main.o wavelet2d.o
	g++ $^ ${FFTW_LIBS} ${OPENCV_LIBS} -o $@

main2: main2.o wavelet2d.o
	g++ $^ ${FFTW_LIBS} ${OPENCV_LIBS} -o $@

main3: main3.o
	g++ $^ ${OPENCV_LIBS} -o $@

main4: main4.o
	g++ $^ ${OPENCV_LIBS} -o $@

