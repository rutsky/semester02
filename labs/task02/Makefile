#
# Makefile.
# Bob Rutsky <rutsky_v@rambler.ru>
# 05.03.2005
#

PROGRAM_NAME = test
OBJECT_FILES = main.o vector.o heap.o figure_sierpinsky2.o
CC = gcc
LIB_COMPILER_OPTIONS =
COMPILER_OPTIONS = -c -g -Wall -O2 $(LIB_COMPILER_OPTIONS)
LIB_LINK_OPTIONS = -lm
LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

.c.o:
	$(CC) $(COMPILER_OPTIONS) $<

clean:
	rm -rf *.o $(PROGRAM_NAME)

main.o: main.c

vector.o: vector.c vector.h

heap.o: heap.c heap.h

figure_sierpinsky2.o: figure_sierpinsky2.c figure_sierpinsky2.h figure.h geom.h vector.h heap.h

# END OF 'Makefile' FILE
