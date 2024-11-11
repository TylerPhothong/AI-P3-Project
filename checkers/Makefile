# Compiler and flags
CC              := gcc
CFLAGS          := -Wall -ggdb
CPPFLAGS        := -I./ -I/usr/X11R6/include/Xm -I/usr/X11R6/include -I/usr/include/openmotif
LDFLAGS         := -L/usr/X11R6/lib -L /usr/X11R6/LessTif/Motif1.2/lib -lXm -lXaw -lXmu -lXt -lX11 -lICE -lSM -pthread -L/usr/lib64/openmotif/

# Enable graphics if required
 CFLAGS          += -DGRAPHICS

# Targets
all: checkers computer

# Rules to build checkers and computer
checkers: checkers.o graphics.o
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ checkers.o graphics.o ${LDFLAGS}

computer: myprog.o
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ myprog.o

# Object file generation
%.o: %.c
	${CC} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

# Clean up
.PHONY: clean
clean:	
	@-rm -f checkers computer *.o
