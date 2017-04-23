# IOS - Project 2
# Author: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>

CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDLIBS = -lpthread

EXECUTABLE = proj2
OBJS = $(shell $(CC) -MM *.c | grep ':' | cut -d ':' -f1 | tr '\n' ' ')
PACK = $(EXECUTABLE).zip


.PHONY: all dependencies run pack clean clean_res test


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJS)


# generated dependencies by gcc, see target dependencies
adult_generator.o: adult_generator.c adult_generator.h resources.h
argument_processor.o: argument_processor.c argument_processor.h
child_generator.o: child_generator.c child_generator.h resources.h
proj2.o: proj2.c argument_processor.h resources.h adult_generator.h child_generator.h
resources.o: resources.c resources.h


dependencies:
	$(CC) -MM *.c


run: $(EXECUTABLE)
	./$<


pack:
	zip $(PACK) *.h *.c Makefile


clean:
	rm -f $(EXECUTABLE) *.o $(PACK)

clean_res: scripts/clean.sh
	chmod +x $<
	./$< $(EXECUTABLE)

test: scripts/check-syntax.sh
	chmod +x $<
	./$< $(EXECUTABLE).out
