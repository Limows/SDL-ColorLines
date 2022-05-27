#############################################################################
# Makefile for building ColorLines.ezx
#    Project: ColorLines.ezx
#    Template: app
#############################################################################

####### Compiler, tools and options

SDK_EZX_LIB_DIR = $(MOTOEZXPATH)/lib
SDK_INC_DIR = $(MOTOEZXPATH)/include
SDK_SDL_INC_DIR = $(MOTOEZXPATH)/include/SDL
CC	=	arm-linux-gnu-gcc
CXX	=	arm-linux-gnu-g++
CFLAGS	=	-pipe `sdl-config --cflags` -Wall -W -O2 -mcpu=iwmmxt -mtune=iwmmxt -DNO_DEBUG
CXXFLAGS=	-pipe `sdl-config --cflags` -DQWS -fno-exceptions -fno-rtti -Wall -W -O2 -mcpu=iwmmxt -mtune=iwmmxt -DNO_DEBUG
INCPATH	=	-I $(SDK_INC_DIR) -I $(SDK_SDL_INC_DIR)
LINK	=	arm-linux-gnu-g++
LFLAGS	=	`sdl-config --libs`
LIBS	=	$(SUBLIBS) -L $(SDK_EZX_LIB_DIR) -lSDL -lSDLmain -lezxappbase -lpthread -lz -lfreetype -lSDL_mixer -lmad -lvorbisfile -lvorbis -logg -lpng -ljpeg
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	
SOURCES =	main.cpp
OBJECTS =	main.o
INTERFACES =
PLATFORM = EZX
SRCMOC	=	
OBJMOC	=	
APPDIR	=	ColorLines
TARGET	=	ColorLines/ColorLines.ezx
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
	$(TAR) $(APPDIR)-$(PLATFORM).pkg $(APPDIR)
	$(TAR) $(APPDIR)-$(PLATFORM).mpkg --numeric-owner --owner=2000 --group=233 $(APPDIR)
	$(GZIP) $(APPDIR)-$(PLATFORM).pkg
	$(GZIP) $(APPDIR)-$(PLATFORM).mpkg
	-mv $(APPDIR)-$(PLATFORM).pkg.gz $(APPDIR)-$(PLATFORM).pkg
	-mv $(APPDIR)-$(PLATFORM).mpkg.gz $(APPDIR)-$(PLATFORM).mpkg


clean:
	-rm -f $(OBJECTS) $(TARGET) $(APPDIR)-$(PLATFORM).pkg $(APPDIR)-$(PLATFORM).mpkg
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

main.o: main.cpp

