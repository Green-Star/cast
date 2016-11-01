PROJECTNAME=client

OBJDIR=../../obj
BINDIR=../../bin
HEADDIR=./inc
_HEADERS=client.h socket.h
_OBJ=client.o socket.o

CC=gcc
CFLAGS=-Wall -Wextra -O2 -g -pthread -I $(HEADDIR)

all: $(BINDIR)/$(PROJECTNAME)

#$(BINDIR)/pere: $(OBJDIR)/pere.o
#	$(CC) $(CFLAGS) $< -o $@
#
#$(BINDIR)/fils: $(OBJDIR)/fils.o
#	$(CC) $(CFLAGS) $< -o $@

HEADERS=$(patsubst %,$(HEADDIR)/%,$(_HEADERS))
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@  

$(BINDIR)/$(PROJECTNAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f $(OBJDIR)/$(_OBJ) $(BINDIR)/$(PROJECTNAME)

rebuild: clear clean all 

clear:
	clear
