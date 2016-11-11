PROJECTNAME=client

OBJDIR=./obj
BINDIR=./bin
HEADDIR=./inc
_HEADERS=$(ls ($HEADDIR)/*)
_OBJ=client.o config.o shakehands_client.o

CC=gcc
CFLAGS=-Wall -Wextra -O2 -g -pthread -I $(HEADDIR)

all: $(BINDIR)/$(PROJECTNAME)

HEADERS=$(patsubst %,$(HEADDIR)/%,$(_HEADERS))
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

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
