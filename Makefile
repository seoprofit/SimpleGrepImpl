CC=gcc 
CFLAGS=-Wextra -Wall -Werror -std=c11

build: s21_grep

main.o: grep.c
	$(CC) $(CFLAGS) -c grep.c

s21_grep: grep.o
	$(CC) $(CFLAGS) grep.o -o s21_grep

clean: 
	rm grep.o s21_grep

test:
	./test_grep.sh

rebuild: clean build
	
