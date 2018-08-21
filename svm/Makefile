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

#empty.

################
##
## Main valuable
##
################

OBJS=main.o \
    svm.o \
    svm-predict.o \
    svm-scale.o \
    svm-train.o
OBJSD=$(subst .o,$(DEBUG_SUFFIX).o,$(OBJS))

CPPFLAGS=$(addprefix -I,$(INC_PATH))
CCFLAGS=
CXXFLAGS=-O0 -w -fmessage-length=0 $(CPPFLAGS)
LDFLAGS=$(addprefix -L,$(LIB_PATH))

TARGETNAME=svmdemo
TARGET=$(TARGETNAME)$(EXTENSION)
TARGETD=$(TARGETNAME)$(DEBUG_SUFFIX)$(EXTENSION)

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

#all: debug release
all: debug

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
