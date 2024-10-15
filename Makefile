CC=gcc
DEPS = hashTable.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -Wall -Wextra -Werror -g

hash_table: hashTable.o
	$(CC) -o hashTable hashTable.o 