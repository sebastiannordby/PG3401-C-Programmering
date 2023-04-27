 #headerfiles - remember to create dir
INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

# output files - remember to create dir
OBJDIR = obj

 # list all header files on the line below
_DEPS = integer-utils.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

#replace source files in the line below, 
#but with the .o - extension
# like: program.c -> program.o
_OBJS = program.o eksamen_v23_oppgave2_abun.o eksamen_v23_oppgave2_cube.o
_OBJS += eksamen_v23_oppgave2_def.o eksamen_v23_oppgave2_fib.o eksamen_v23_oppgave2_kvad.o
_OBJS += eksamen_v23_oppgave2_odd.o eksamen_v23_oppgave2_perf.o eksamen_v23_oppgave2_prim.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#replace hello with the output file name
hello: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~