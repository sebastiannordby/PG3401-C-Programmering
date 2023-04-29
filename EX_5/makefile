 #headerfiles - remember to create dir
INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
#-lsocket
# output files - remember to create dir
OBJDIR = obj

 # list all header files on the line below
_DEPS = 
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

#replace source files in the line below, 
#but with the .o - extension
# like: program.c -> program.o
_OBJS = program.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#replace hello with the output file name
ex5-program: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~