CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all: display_final


###

display_final:	display_final.o
	$(CC) $(LFLAGS) -o display_final display_final.o

display_final.o:	display_final.c
	$(CC) $(CFLAGS) -c display_final.c



###

submit:	display_final.c README Makefile
	tar -czvf hw5_submit.tar.gz display_test.c display_final.c display_cool.c README Makefile


clean:
	rm -f *~ *.o display_final
