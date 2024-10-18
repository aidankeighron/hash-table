project = hashTable

objs = $(project).o

CC=gcc
CFLAGS := $(CFLAGS) -Wall -Wextra -I/usr/include/python3.10 -lpython3.10

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
	rm -f $(project).so
	rm -f tester
	for file in $(CLEANEXTS); do rm -f *.$$file; done

.PHONY: memory
memory:
	valgrind --tool=memcheck --leak-check=full ./$(outputfile)

.PHONY: python
python:
	gcc -shared -o hashTable.so -fPIC python.c -I/usr/include/python3.10 -lpython3.10