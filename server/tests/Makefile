CC=gcc
CFLAGS=-Wall
INC=../inc/
COMMON_INC=../../common/inc/
INCLUDES=-I$(INC) -I$(COMMON_INC)
BINDIR=bin

$(BINDIR)/write_pipe_test: write_pipe_test.c
	$(CC) $(CFLAGS) $^ ../obj/write_pipe.o $(INCLUDES) -o $@ 

all: $(BINDIR)/write_pipe_test
