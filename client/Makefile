PROJECTNAME=client

OBJDIR=./obj
BINDIR=./bin
HEADDIR=./inc
COMMONDIR=../common
HEADCOMMONDIR=$(COMMONDIR)/inc
_HEADERS=$(ls ($HEADDIR)/*)
_OBJ=client.o config.o shakehands_client.o common.o

CC=gcc
CFLAGS=-Wall -Wextra -O2 -g -pthread -I$(HEADDIR) -I$(HEADCOMMONDIR)

all: $(BINDIR)/$(PROJECTNAME)

HEADERS=$(patsubst %,$(HEADDIR)/%,$(_HEADERS))
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/common.o: $(COMMONDIR)/common.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@  

$(BINDIR)/$(PROJECTNAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f $(patsubst %,$(OBJDIR)/%,$(_OBJ)) $(BINDIR)/$(PROJECTNAME)

rebuild: clear clean all 

clear:
	clear
