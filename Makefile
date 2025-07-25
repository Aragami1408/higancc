CC=gcc
CFLAGS=-std=c11 -g -fno-common -Wall -Wno-switch
LDFLAGS=-lm

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

higancc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): src/higancc.h

clean:
	rm -rf higancc src/*.o

.PHONY: clean

