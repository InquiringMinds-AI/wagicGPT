ifeq ($(MAKECMDGOALS),debug)
DEBUG = true
endif

ifeq ($(MAKECMDGOALS),log)
DOLOG = true
endif


GENERIC_OBJS = src/JApp.o \
	src/JGE.o src/JGui.o src/JLBFont.o \
    src/JLogger.o \
	src/JGameObject.o \
	src/JFileSystem.o \
	src/JNetwork.o \
	src/zipFS/zfsystem.o src/zipFS/ziphdr.o src/zipFS/zstream.o \
	src/JSprite.o src/Vector2D.o \
	src/tinyxml/tinystr.o src/tinyxml/tinyxml.o \
	src/tinyxml/tinyxmlparser.o src/tinyxml/tinyxmlerror.o \
	src/Encoding.o \
	src/JMD2Model.o
PSP_OBJS = src/JSocket.o src/JGfx.o src/JSfx.o src/JAudio.o src/JMP3.o src/decoder_prx.o src/main.o src/vram.o
LINUX_OBJS = src/pc/JGfx.o src/pc/JSfx.o


HGE_OBJS = src/hge/hgecolor.o src/hge/hgeparticle.o \
       src/hge/hgerect.o src/hge/hgevector.o \
       src/hge/hgedistort.o src/hge/hgefont.o


CXXFLAGS = -W -Wall -Wno-unused
CXXFLAGS += -DTIXML_USE_STL
CXXFLAGS += -Wno-deprecated-declarations -Wno-sign-compare
# Codebase predates C++17 (random_shuffle etc.); modern GCC defaults past it.
CXXFLAGS += -std=gnu++14
CXXFLAGS += $(EXTRA_DEFS)
CFLAGS += $(EXTRA_DEFS)

ifdef DEBUG
CXXFLAGS += -ggdb3
endif

ifdef DOLOG
CXXFLAGS += -DDOJLOG
endif

# Determination of target.
# TARGET_ARCHITECTURE variable will then be set to either linux or psp.

RESULT = $(shell psp-config --psp-prefix 2> Makefile.cache)
ifeq ($(RESULT),)
DEFAULT_RULE = linux
TARGET_ARCHITECTURE = linux
else
DEFAULT_RULE = 3xx
TARGET_ARCHITECTURE = psp
TARGET_LIB = libjge300.a
endif

ifeq ($(MAKECMDGOALS),linux)
DEFAULT_RULE = linux
TARGET_ARCHITECTURE = linux
endif
ifeq ($(MAKECMDGOALS),3xx)
DEFAULT_RULE = 3xx
endif

ifeq ($(DEFAULT_RULE),3xx)
TARGET_ARCHITECTURE = psp
TARGET_LIB = libjge300.a
CXXFLAGS += -DDEVHOOK -DPSPFW3XX
PSP_FW_VERSION=371
endif

ifeq ($(TARGET_ARCHITECTURE),psp)
PSPSDK = $(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)
OBJS = $(GENERIC_OBJS) $(PSP_OBJS)
TARGET_HGE = libhgetools.a
INCDIR = include/psp include/psp/freetype2 ../Boost src/zipFS
CXXFLAGS += -O2 -G0 -DPSP
#Real-hardware getenv returns garbage (uninitialized environ) - neuter it.
CXXFLAGS += -include include/psp_noenv.h
CFLAGS += -include include/psp_noenv.h
LIBDIR = lib/psp
endif
ifeq ($(TARGET_ARCHITECTURE),linux)
OBJS = $(GENERIC_OBJS) $(LINUX_OBJS)
TARGET_LIB = libjge.a
TARGET_HGE = libhgetools.a
INCDIR = $(shell freetype-config --cflags 2> /dev/null) -I/usr/X11/include -I/usr/include/boost -Isrc/zipFS -Iinclude/
CXXFLAGS += -DLINUX -DNETWORK_SUPPORT $(FMOD)
CXXFLAGS += $(INCDIR)
LIBDIR = lib/linux
endif

# Set definitive values for variables.
TARGET_LIB := $(LIBDIR)/$(TARGET_LIB)
TARGET_HGE := $(LIBDIR)/$(TARGET_HGE)

ifeq ($(TARGET_ARCHITECTURE),psp)
include $(PSPSDK)/lib/build.mak
endif

all: $(DEFAULT_RULE) Makefile.$(TARGET_ARCHITECTURE) hge

debug: $(DEFAULT_RULE) hge

linux: $(TARGET_LIB) hge

3xx: $(TARGET_LIB)

install: $(TARGET_LIB) hge

hge: $(TARGET_HGE)

ifeq ($(TARGET_ARCHITECTURE),psp)
#ar's r (replace-or-insert) never EVICTS: members whose sources left the objects
#list survive in the archive forever, so builds of the same commit differ by
#session history. This is the stale-library defect that poisoned the 2026-08-03
#bisect (one binary carried two definitions of the same macro). Delete the
#archive before rebuilding it so its contents are exactly the current objects.
$(TARGET_HGE): $(HGE_OBJS)
	$(RM) -f $(TARGET_HGE)
	$(AR) r $(TARGET_HGE) $(HGE_OBJS)

#Same defect, same fix for libjge300.a - this overrides the PSPSDK build.mak
#recipe ($(AR) cru with no delete); the "overriding recipe" warning make prints
#is expected and deliberate.
$(TARGET_LIB): $(OBJS)
	$(RM) -f $(TARGET_LIB)
	$(AR) cru $(TARGET_LIB) $(OBJS)

#build.mak's clean removes $(FINAL_TARGET) and $(OBJS) but knows nothing about
#the hge archive OR its objects; EXTRA_CLEAN is its extension point. With
#this, `make clean` inside the pspdev container is a REAL clean - no more
#hand-rm'ing .a or .o files.
EXTRA_CLEAN += $(TARGET_HGE) $(HGE_OBJS)
endif

ifeq ($(TARGET_ARCHITECTURE),linux)


$(TARGET_LIB): $(OBJS)
	ar r $(TARGET_LIB) $(OBJS)

$(TARGET_HGE): $(HGE_OBJS)
	ar r $(TARGET_HGE) $(HGE_OBJS)

clean:
	$(RM) -f $(OBJS) $(HGE_OBJS) Makefile.$(TARGET_ARCHITECTURE)
endif

Makefile.psp:
	echo #Makefile.psp > $@

Makefile.linux:
	$(CXX) -o /dev/null src/testfeatures.c -L$(LIBDIR) -lfmod-3.75 > /dev/null 2>&1 ; if [ "0" = "$$?" ]; then echo 'FMOD=-DWITH_FMOD'; else echo 'FMOD=-DWITHOUT_FMOD'; fi > $@

-include Makefile.$(TARGET_ARCHITECTURE)
