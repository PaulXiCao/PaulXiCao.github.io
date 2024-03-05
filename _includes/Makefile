CC = gcc
CFLAGS = -fsyntax-only -Wall -Wextra -std=c11

.PHONY: default

default:
	$(foreach					\
		file,					\
		$(wildcard *.c),		\
		$(CC) $(CFLAGS) $(file)	\
	)
