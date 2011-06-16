
CC = gcc

shutbox: shutbox.c 
	$(CC) -Wall -g shutbox.c -o shutbox `pkg-config --cflags gtk+-2.0` \
	`pkg-config --libs gtk+-2.0`

clean: 
	rm -f *.o shutbox

install: shutbox
	install shutbox /usr/local/bin
