CC            = gcc
CFLAGS        = 
DEST          = ./
LDFLAGS       = 
LIBS          = 
OBJS          = main.o cc_tkn.o cc_pars.o cc_print.o cc_eval.o
PROGRAM       = mycc
PROGRAM_DEBUG = debug_mycc

all:  $(PROGRAM)

$(PROGRAM):     $(OBJS)
				$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM); touch *.c

debug: CFLAGS = -DDEBUG
debug:     $(OBJS)
				$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM_DEBUG); touch *.c

clean:;         rm -f *.o *~ $(PROGRAM) $(PROGRAM_DEBUG)

install:        $(PROGRAM)
				install -s $(PROGRAM) $(DEST)