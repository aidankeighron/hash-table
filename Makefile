CC=gcc
DEPS = hash_table.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -Wall -Wextra -Werror

hash_table: hash_table.o
	$(CC) -o hash_table hash_table.o 