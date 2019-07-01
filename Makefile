
CROSS:=
CC:=$(CROSS)gcc
CFLAGS:=-I./
OBJ:=$(wildcard *.c)

BIN:=parser

.PHONY:all
.PHONY:clean

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS)

clean:
	/bin/rm -rf *.o;/bin/rm $(BIN)
