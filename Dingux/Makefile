#############################################################################
# Makefile for building ColorLines.elf
#    Project: ColorLines.elf
#    Template: app
#############################################################################

####### Compiler, tools and options

SDK_LIB_DIR = $(RZX50PATH)/lib
SDK_INC_DIR = $(RZX50PATH)/include
SDK_USR_LIB_DIR = $(RZX50PATH)/usr/lib
SDK_USR_INC_DIR = $(RZX50PATH)/usr/include
SDK_SDL_INC_DIR = $(RZX50PATH)/usr/include/SDL
CC	=	mipsel-linux-gcc
CXX	=	mipsel-linux-g++
CFLAGS	=	-pipe -D_GNU_SOURCE=1 -D_REENTRANT -Wall -W -O2 -mips32 -mtune=mips32 -DNO_DEBUG
CXXFLAGS=	-pipe -D_GNU_SOURCE=1 -D_REENTRANT -fno-exceptions -fno-rtti -Wall -W -O2 -mips32 -mtune=mips32 -DNO_DEBUG
INCPATH	=	-I $(SDK_USR_INC_DIR) -I $(SDK_SDL_INC_DIR) -I $(SDK_INC_DIR)
LINK	=	mipsel-linux-g++
LFLAGS	=	`sdl-config --libs`
LIBS	=  -L $(SDK_LIB_DIR) -L $(SDK_USR_LIB_DIR) -lSDL -lSDLmain -lpthread -lz -lSDL_mixer

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	
SOURCES =	main.cpp
OBJECTS =	main.o
INTERFACES =
PLATFORM = Dingux
SRCMOC	=	
OBJMOC	=
APPDIR	=	ColorLines
TARGET	=	ColorLines/ColorLines.elf
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

dist:
	$(TAR) $(APPDIR)-$(PLATFORM).tar $(APPDIR)
	$(GZIP) $(APPDIR)-$(PLATFORM).tar

clean:
	-rm -f $(OBJECTS) $(TARGET) $(APPDIR)-$(PLATFORM).tar.gz
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

main.o: main.cpp

