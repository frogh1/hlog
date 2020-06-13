#make platform=[windows|linux]

RM := rm -rf
CC := 
OPT :=

appname := testlog
TARGET :=


PLATFORM := ${platform}

ifeq ($(PLATFORM),windows)
	CC := i686-w64-mingw32-gcc
	OPT = -DWIN32 
	TARGET = ${appname}.exe
else
	CC := gcc
	OPT = -DLINUX
	TARGET = ${appname}
endif

INCDIR := -I.
LIBDIR := -L.

CFLAGS := -w -W -Werror -Wall
LDFLAGS := 

ifeq ($(PLATFORM),windows)
else
	LDFLAGS += -lpthread
endif

ACTION = release

ifeq ($(ACTION),release)
CFLAGS += -O2
else
CFLAGS += -g
endif


GEN_SRC := $(shell ls *.c)
GEN_OBJ := $(patsubst %.c,%.o, $(GEN_SRC))

ALL_OBJ= main.o log.o 

.PHONY: all clean

#all: loglib ${TARGET}
all: ${TARGET}


${TARGET}:${ALL_OBJ}
	$(CC) $(CFLAGS) $(OPT) $(LIBDIR) $(ALL_OBJ) -LS -o $(TARGET) $(LDFLAGS)

# loglib:
#     @cd log && make


%.o: %.c
	$(CC) -c ${CFLAGS} ${OPT} $< -o $@

clean:
	$(RM) ${TARGET}  ${TARGET}.exe
	$(RM) *.o

