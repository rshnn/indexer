CC = gcc
CFLAGS = -Wall -Werror -g

all: index 

index: indexer.o tokenizer.o libsl.a
	$(CC) $(CFLAGS) -o index $^

indexer.o:	indexer.c
	$(CC) $(CFLAGS) -c $^

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c $^

libsl.a:	sorted-list.o
	ar rv libsl.a sorted-list.o

sorted-list.o: 	sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c	


clean:
	rm -f index
	rm -f *o
	rm -f libsl.a