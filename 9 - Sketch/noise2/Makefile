# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# Specify what typing 'make' on its own will compile
default: map

# For Windows, add the .exe extension
ifdef Windows

map: map.c perlin.c display.c
	clang -std=c11 -Wall -pedantic -g $@.c display.c -lSDL2 perlin.c -o $@.exe

# For Linux/MacOS, include the advanced debugging options
else

map: map.c perlin.c display.c
	clang -std=c11 -Wall -pedantic -g $@.c display.c -lSDL2 perlin.c -o $@ \
	    -fsanitize=undefined -fsanitize=address
		
endif
