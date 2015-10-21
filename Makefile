CC = gcc
CFLAGS = -Wall -Werror -g

all: index 

index: main.o tokenizer.o
	$(CC) $(CFLAGS) -o index $^

main.o:	main.c
		$(CC) $(CFLAGS) -c $^

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c $^


clean:
	rm -f index
	rm -f *o