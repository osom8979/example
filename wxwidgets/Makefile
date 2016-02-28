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

WX_CPPFLAGS=
WX_VER=3.0
WX_HOST=-x86_64-w64-mingw32
WX_LIBEXT=.dll
WX_SUFFIX:=$(WX_VER)$(WX_HOST)$(WX_LIBEXT)
WX_LIBS=\
    wx_baseu-$(WX_SUFFIX) \
    wx_baseu_net-$(WX_SUFFIX) \
    wx_baseu_xml-$(WX_SUFFIX) \
    wx_mswu_adv-$(WX_SUFFIX) \
    wx_mswu_aui-$(WX_SUFFIX) \
    wx_mswu_core-$(WX_SUFFIX) \
    wx_mswu_gl-$(WX_SUFFIX) \
    wx_mswu_html-$(WX_SUFFIX) \
    wx_mswu_media-$(WX_SUFFIX) \
    wx_mswu_propgrid-$(WX_SUFFIX) \
    wx_mswu_qa-$(WX_SUFFIX) \
    wx_mswu_ribbon-$(WX_SUFFIX) \
    wx_mswu_richtext-$(WX_SUFFIX) \
    wx_mswu_stc-$(WX_SUFFIX) \
    wx_mswu_webview-$(WX_SUFFIX) \
    wx_mswu_xrc-$(WX_SUFFIX)
WX_LDFLAGS=$(addprefix -l,$(WX_LIBS))

################
##
## Main valuable
##
################

OBJS=main.o manager.o \
    t01.o t02.o t03.o t04.o t05.o t06.o t07.o t08.o t09.o t10.o \
    t11.o t12.o t13.o t14.o t15.o t16.o t17.o t18.o t19.o t20.o \
    t21.o t22.o t23.o t24.o t25.o t26.o t27.o t28.o t29.o t30.o \
    t31.o t32.o t33.o t34.o t35.o t36.o t37.o t38.o t39.o t40.o \
    t41.o t42.o t43.o t44.o t45.o t46.o t47.o t48.o t49.o t50.o \
    t51.o t52.o

OBJSD=$(subst .o,$(DEBUG_SUFFIX).o,$(OBJS))

CPPFLAGS=$(addprefix -I,$(INC_PATH)) $(WX_CPPFLAGS)
CCFLAGS=
CXXFLAGS=-O2 -Wall -fmessage-length=0 $(CPPFLAGS)
LDFLAGS=$(addprefix -L,$(LIB_PATH)) $(WX_LDFLAGS)

TARGETNAME=wxWidgets
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
