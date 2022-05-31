# Define the applications properties here:

APP_NAME = ColorLinesSdl



# Define the compiler settings here:

CPP       = mipsel-linux-g++
CC        = mipsel-linux-gcc
LD        = mipsel-linux-ld

SOURCE    = .

INCLUDE   = -I$(DINGOO_SDK)/include -I$(DINGOO_SDK)/include/SDL -I../lib -I$(MIPSTOOLS)/mipsel-linux/include -I$

W_OPTS    = -Wall -Wextra -finline-functions -fomit-frame-pointer -msoft-float -fno-builtin -fno-exceptions
CPP_OPTS  = -G0 -O3 $(INCLUDE) -mips32 -mno-abicalls -fno-pic $(W_OPTS) -D_DEBUG -DMPU_JZ4740 -c
CC_OPTS   = -G0 -O3 $(INCLUDE) -mips32 -mno-abicalls -fno-pic $(W_OPTS) -D_DEBUG -DMPU_JZ4740 -DNOSOUND -DEMBEDDED -c
CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_

LIB_PATH  = $(DINGOO_SDK)/lib
LIBS      = -lSDL -lpng -lsml -lz -lm -lc -ljz4740 -lgcc

LD_SCRIPT = $(DINGOO_SDK)/lib/dingoo.xn
LD_OPTS   = -nodefaultlibs --script $(LD_SCRIPT) -L$(LIB_PATH) $(LIBS) -o $(APP_NAME).elf



# Find all source files

SRC_CPP = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.cpp))
SRC_C   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.c))
SRC_S   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.S))
OBJ_CPP = $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))
OBJ_S   = $(patsubst %.S, %.o, $(SRC_S))
OBJ     = $(OBJ_CPP) $(OBJ_C) $(OBJ_S)



# Compile rules.

.PHONY : all

all : $(APP_NAME).app

$(APP_NAME).app : $(APP_NAME).elf
	$(DINGOO_SDK)/tools/elf2app/elf2app $(APP_NAME)

$(APP_NAME).elf : $(OBJ)
	$(LD) $(OBJ) $(LD_OPTS)

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<



# Clean rules

.PHONY : clean

clean :
	rm -f $(OBJ) $(APP_NAME).elf $(APP_NAME).bin $(APP_NAME).app