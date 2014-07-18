#
#
#

CC=gcc
CFLAGS=-O2 -D_GNU_SOURCE -DGETPIR -Wall

all: testio

testio: testio.o
	$(CC) $(CFLAGS) -o getpir $^

clean:
	rm -f testio *.o *~

distclean: clean


