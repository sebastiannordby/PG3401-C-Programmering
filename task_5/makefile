# This makefile is provided by lecturer from lecture 4, slide 65, 
# but with a few modifications. 

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
OBJDIR = obj

 # list all header files on the line below
_DEPS = server.h client.h reverse-shell.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

#replace source files in the line below, 
#but with the .o - extension
# like: program.c -> program.o
_OBJS = program.o server.o client.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rev-shell: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~