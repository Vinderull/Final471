CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all: final


###

display_final:	final.o
	$(CC) $(LFLAGS) -o final final.o

display_final.o:	final.c
	$(CC) $(CFLAGS) -c final.c



###

submit:	final.c Makefile README rfcomm-client.py rfcomm-server.py TSLHeader.h
	tar -czvf final_project.tar.gz final.c Makefile rfcomm-client.py rfcomm-server.py TSLHeader.h README


clean:
	rm -f *~ *.o final
