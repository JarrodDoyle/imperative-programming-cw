# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# Specify what typing 'make' on its own will compile
default: maze

%: %.c
	clang -std=c11 -Wall -pedantic -g $@.c -o $@ \
	    -fsanitize=undefined -fsanitize=address
