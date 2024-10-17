project = hashTable

objs = $(project).o

CC=gcc
CFLAGS := $(CFLAGS) -Wall -Wextra

CLEANEXTS   = o

outputfile  = $(project)

# Default target
.PHONY: all
all: $(outputfile)

$(outputfile): $(objs)
		gcc -o $(outputfile) $(objs) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(outputfile)
	rm -f tester
	for file in $(CLEANEXTS); do rm -f *.$$file; done

.PHONY: memory
memory:
	valgrind --tool=memcheck --leak-check=full ./$(outputfile)