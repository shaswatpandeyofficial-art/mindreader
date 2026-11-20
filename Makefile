CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic

.PHONY: all clean
all: mind

libmind.so: mind.c mind.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ mind.c

mind: main.c mind.h libmind.so
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ main.c \
		-L. -lmind -Wl,-rpath,'$$ORIGIN'

clean:
	$(RM) mind libmind.so
