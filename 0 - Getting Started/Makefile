# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# For Windows, add the .exe extension
ifdef Windows

square: square.c
	clang -std=c11 -Wall -pedantic -g square.c -o square.exe

# For Linux/MacOS, include the advanced debugging options
else

square: square.c
	clang -std=c11 -Wall -pedantic -g square.c -o square \
	    -fsanitize=undefined -fsanitize=address

endif
