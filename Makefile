filebin: main.c
	$(CC) -Wall -o filebin main.c

clean:
	rm -f filebin

test:
	./filebin Makefile
