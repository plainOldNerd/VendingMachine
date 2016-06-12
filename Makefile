########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : Andrew Mouat
# Student Number   : 2109279U
# Course Code      : COSC1076 Advanced Programming Techniques
# Program Code     : BP096 B. Software Engineering
# Start up code provided by Paul Miller and Virginia King
########################################################################

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Please add any additional files you add to your 
# project and delete ones your remove. Do not submit your .dat files, 
# your .bin files, containing directories. We only want the files that 
# are part of your implementation
########################################################################

CC=gcc
CFLAGS=-ansi -pedantic -Wall -g
LFLAGS=
TARGETS=vm
HEADERS=*.h

vm:*.o
	$(CC) $(LFLAGS) $^ -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $^

.PHONY:clean
clean:
	rm *.o *.gch

.PHONY:archive
archive:
	zip 2109279U-a2 *.c *.h Makefile
