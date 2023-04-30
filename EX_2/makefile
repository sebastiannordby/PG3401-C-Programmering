# This makefile is provided by lecturer from lecture 4, slide 65, 
# but with a few modifications. 
INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -lm
CFLAGS += -I$(INCLDIR)
OBJDIR = obj

# list all header files on the line below
_DEPS = integer-utils.h integer-metadata.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

#replace source files in the line below, 
#but with the .o - extension
# like: program.c -> program.o
_OBJS = program.o abun.o cube.o
_OBJS += def.o fib.o kvad.o
_OBJS += odd.o perf.o prim.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

number-program: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~