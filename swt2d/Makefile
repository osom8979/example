.SUFFIXES : .cpp .o .lo
.PHONY: clean

CC=gcc
CXX=g++
LD=ld

## Only Windows Environment Variable.
INC_PATH=$(subst ;, ,$(subst //,/,$(subst \,/,$(CPLUS_INCLUDE_PATH))))
LIB_PATH=$(subst ;, ,$(subst //,/,$(subst \,/,$(LIBRARY_PATH))))

OBJS=\
    swt2Ddemo.o \
    wavelet2d.o

LIBS=\
    -lopencv_calib3d249.dll \
    -lopencv_contrib249.dll \
    -lopencv_core249.dll \
    -lopencv_features2d249.dll \
    -lopencv_flann249.dll \
    -lopencv_gpu249.dll \
    -lopencv_highgui249.dll \
    -lopencv_imgproc249.dll \
    -lopencv_legacy249.dll \
    -lopencv_ml249.dll \
    -lopencv_nonfree249.dll \
    -lopencv_objdetect249.dll \
    -lopencv_ocl249.dll \
    -lopencv_photo249.dll \
    -lopencv_stitching249.dll \
    -lopencv_superres249.dll \
    -lopencv_ts249 \
    -lopencv_video249.dll \
    -lopencv_videostab249.dll \
    -lfftw3

CPPFLAGS=$(addprefix -I,$(INC_PATH))
CXXFLAGS=-Wall -g -O2 -fmessage-length=0
LDFLAGS=-static $(addprefix -L,$(LIB_PATH)) $(LIBS)

TARGET=swt2Ddemo.exe

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) -c $<
