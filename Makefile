############
#Makefile
#Lucas Manker
#COSC 3750 Spring 2020
#Homework #8
#4/20/20
#
#Makefile for hw9
###########

CC=gcc
CFLAGS=-ggdb -Wall -Wno-unused-function 

.PHONY: clean

wyshell: wyscanner.c wyshell.c
	$(CC) $(CFLAGS) wyshell.c wyscanner.c -o wyshell


clean:
	/bin/rm -f wyshell
