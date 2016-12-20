OBJDIR=./obj
BINDIR=./bin
HEADDIR=./inc
COMMONDIR=../common
HEADCOMMONDIR=$(COMMONDIR)/inc
_HEADERS=$(ls ($HEADDIR)/*)
_OBJ_MAIN=main_server.o
_OBJ_CAST=cast_server.o shakehands_server.o cast_receiver.o cast_streaming.o common.o command_vlc.o write_pipe.o

CC=gcc
CFLAGS=-Wall -Wextra -g -pthread -I$(HEADDIR) -I$(HEADCOMMONDIR)

all: $(BINDIR)/main_server $(BINDIR)/cast_server 

HEADERS=$(patsubst %,$(HEADDIR)/%,$(_HEADERS))
OBJ_MAIN=$(patsubst %,$(OBJDIR)/%,$(_OBJ_MAIN))
OBJ_CAST=$(patsubst %,$(OBJDIR)/%,$(_OBJ_CAST))

$(OBJDIR)/common.o: $(COMMONDIR)/common.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@  

$(BINDIR)/main_server: $(OBJ_MAIN)
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/cast_server: $(OBJ_CAST)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f $(patsubst %,$(OBJDIR)/%,$(_OBJ_MAIN))
	rm -f $(patsubst %,$(OBJDIR)/%,$(_OBJ_CAST))
	rm -f $(BINDIR)/main_server $(BINDIR)/cast_server

rebuild: clear clean all 

clear:
	clear
