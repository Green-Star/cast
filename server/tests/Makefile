CC=gcc
CFLAGS=-Wall -g
INC=../inc/
COMMON_INC=../../common/inc/
INCLUDES=-I$(INC) -I$(COMMON_INC)
BINDIR=bin

$(BINDIR)/write_pipe.o:
	$(CC) $(CFLAGS) -c ../write_pipe.c $(INCLUDES) -o $@
$(BINDIR)/command_vlc.o:
	$(CC) $(CFLAGS) -c ../command_vlc.c $(INCLUDES) -o $@

$(BINDIR)/write_pipe_test: write_pipe_test.c $(BINDIR)/write_pipe.o
	$(CC) $(CFLAGS) $^ ../obj/write_pipe.o $(INCLUDES) -o $@ 

$(BINDIR)/command_vlc_test: command_vlc_test.c $(BINDIR)/write_pipe.o $(BINDIR)/command_vlc.o
	$(CC) $(CFLAGS) $^ $(INCLUDES) -o $@

all:
	$(BINDIR)/write_pipe_test
	$(BINDIR)/command_vlc_test
