CFLAGS		=	-g -Wall -I/ADT
ADTFLAGS	=	-g -Wall -Iinclude

all: dine holes clean

dine:
	gcc -g -Wall -pthread dine.c -o dine

holes: libs main
	gcc holes.o LinkedList.o -o holes
	
main:
	gcc $(CFLAGS) -c holes.c
	
libs: LinkedList

LinkedList:
	gcc $(ADTFLAGS) -c ADT/LinkedList.c

clean:
	@ rm *.o
