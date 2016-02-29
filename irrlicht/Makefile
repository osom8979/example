.SUFFIXES : .cpp .o .lo
.PHONY: default all debug release clean

CC=gcc
CXX=g++
LD=ld

DEBUG_FLAG=-g
DEBUG_SUFFIX=_g

#HOST=UNIX
HOST=WINDOWS

## OS Specialize
ifeq ($(HOST),UNIX)
INC_PATH=$(subst ;, ,$(subst //,/,$(CPLUS_INCLUDE_PATH)))
LIB_PATH=$(subst ;, ,$(subst //,/,$(LIBRARY_PATH)))
PATH_SEPARATOR:=:
FILE_SEPARATOR:=/
EXTENSION=
else
INC_PATH=$(subst ;, ,$(subst //,/,$(subst \,/,$(CPLUS_INCLUDE_PATH))))
LIB_PATH=$(subst ;, ,$(subst //,/,$(subst \,/,$(LIBRARY_PATH))))
PATH_SEPARATOR:=;
FILE_SEPARATOR:=\\
EXTENSION=.exe
endif

############
##
## Libraries
##
############

IRRLICHT_CPPFLAGS=-D_IRR_STATIC_LIB_
IRRLICHT_LIBS=irrlicht \
    gdi32 winspool comdlg32 ole32 oleaut32 \
    uuid odbc32 odbccp32 opengl32 m winmm
IRRLICHT_LDFLAGS=$(addprefix -l,$(IRRLICHT_LIBS))

################
##
## Main valuable
##
################

OBJS=main.o q3factory.o sound.o \
    t01.o t02.o t03.o t04.o t05.o t06.o t07.o t08.o t09.o t10.o \
    t11.o t12.o t13.o t14.o t15.o t16.o t17.o t18.o t19.o t20.o \
    t21.o t22.o t23.o t24.o t25.o t26.o
OBJSD=$(subst .o,$(DEBUG_SUFFIX).o,$(OBJS))

CPPFLAGS=$(addprefix -I,$(INC_PATH)) $(IRRLICHT_CPPFLAGS)
CCFLAGS=
CXXFLAGS=-O2 -w -fmessage-length=0
LDFLAGS=$(addprefix -L,$(LIB_PATH)) $(IRRLICHT_LDFLAGS)

TARGET=irrlichtdemo$(EXTENSION)
TARGETD=irrlichtdemo$(DEBUG_SUFFIX)$(EXTENSION)

ALL_OBJS=$(OBJS) $(OBJSD)
ALL_TARGETS=$(TARGET) $(TARGETD)

################
##
## Function list
##
################

define LINK
    $($(1)) -o $@ $^ $($(2))
endef

define COMPILE
    $($(1)) -o $@ -c $< $(CPPFLAGS) $($(1)FLAGS)
endef

define DEBUG_COMPILE
    $($(1)) -o $@ -c $< $(DEBUG_FLAG) $(CPPFLAGS) $($(1)FLAGS)
endef

LINK_CC=$(call LINK,CC,LDFLAGS)
LINK_CXX=$(call LINK,CXX,LDFLAGS)
COMPILE_CC=$(call COMPILE,CC)
COMPILE_CXX=$(call COMPILE,CXX)
DEBUG_COMPILE_CC=$(call DEBUG_COMPILE,CC)
DEBUG_COMPILE_CXX=$(call DEBUG_COMPILE,CXX)

##########
##
## Targets
##
##########

default: debug

all: debug release

debug: $(TARGETD)

release: $(TARGET)

clean:
	rm -f $(ALL_OBJS) $(ALL_TARGETS)

$(TARGET): $(OBJS)
	$(LINK_CXX)

$(TARGETD): $(OBJSD)
	$(LINK_CXX)

%_g.o: %.cpp
	$(DEBUG_COMPILE_CXX)

%.o: %.cpp
	$(COMPILE_CXX)
