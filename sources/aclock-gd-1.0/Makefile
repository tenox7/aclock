CC=gcc

all: libcgic.a aclock

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	ar rc libcgic.a cgic.o

aclock: 
	gcc aclock.c -o aclock -L./ -lcgic -lgd -lz -lpng -lm 

clean:
	rm -f *.o *.a aclock

