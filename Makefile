CC=gcc
DEPS = hashTable.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -Wall -Wextra -Werror

hash_table: hashTable.o
	$(CC) -o hashTable hashTable.o 