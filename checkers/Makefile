CC              := gcc
CFLAGS          := -Wall -ggdb
CPPFLAGS        := -I./ -I/usr/X11R6/include/Xm -I/usr/X11R6/include -I/usr/include/openmotif
LDFLAGS         := -L/usr/X11R6/lib -L/usr/X11R6/LessTif/Motif1.2/lib -L/usr/lib64/openmotif/ -lX11 -lXaw -lXmu -lICE -lSM -pthread -lXt -lXm -lXext

all: checkers TylerBot myprog

checkers: checkers.o graphics.o
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ $^ ${LDFLAGS}

TylerBot: TylerBot.o
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ $^ ${LDFLAGS}

myprog: myprog.o
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ $^ ${LDFLAGS}

# Compile object files from .c files
%.o: %.c
	${CC} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

.PHONY: clean
clean:
	@-rm checkers TylerBot myprog *.o
