# IOS - Project 2
# Author: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDLIBS = -lpthread

EXECUTABLE = proj2
OBJS = $(shell $(CC) -MM *.c | cut -d ':' -f1 | tr '\n' ' ')
PACK = $(EXECUTABLE).zip


.PHONY: all dependencies run pack clean


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)


# generated dependencies by gcc, see target dependencies
argument_processor.o: argument_processor.c argument_processor.h
proj2.o: proj2.c argument_processor.h


dependencies:
	$(CC) -MM *.c


run: $(EXECUTABLE)
	./$<


pack:
	zip $(PACK) *.h *.c Makefile


clean:
	rm -f $(EXECUTABLE) *.o $(PACK)
