#############################################################################
# Makefile for building ColorLines
#    Project: ColorLines
#    Template: app
#############################################################################

####### Compiler, tools and options

CC	=	gcc
CXX	=	g++
CFLAGS	=	-pipe -D_GNU_SOURCE=1 -D_REENTRANT -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -D_GNU_SOURCE=1 -D_REENTRANT -fno-exceptions -fno-rtti -Wall -W -O2 -DNO_DEBUG
INCPATH	=
LINK	=	g++
LFLAGS	=	`sdl-config --libs`
LIBS	=   -lSDL -lSDLmain -lpthread -lz -lSDL_mixer -lSDL_image -logg

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	
SOURCES =	main.cpp
OBJECTS =	main.o
INTERFACES =	
PLATFORM = Linux
CPU = `uname -m`
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

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

dist:
	$(TAR) $(APPDIR)-$(PLATFORM)-$(shell uname -m).tar $(APPDIR)
	$(GZIP) $(APPDIR)-$(PLATFORM)-$(shell uname -m).tar

clean:
	-rm -f $(OBJECTS) $(TARGET) $(APPDIR)-$(PLATFORM)-$(shell uname -m).tar.gz
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

main.o: main.cpp

