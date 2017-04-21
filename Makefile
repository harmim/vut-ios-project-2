# IOS - Project 2
# Author: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDLIBS = -lpthread

EXECUTABLE = proj2
OBJS = $(wildcard *.o)
PACK = $(EXECUTABLE).zip


.PHONY: all dependencies run pack clean


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)


# generated dependencies by gcc, see target dependencies
proj2.o: proj2.c


dependencies:
	$(CC) -MM *.c


run: $(EXECUTABLE)
	./$<


pack:
	zip $(PACK) *.h *.c Makefile


clean:
	rm -f $(EXECUTABLE) *.o $(PACK)
