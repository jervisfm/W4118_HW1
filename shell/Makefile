CC := gcc
CFLAGS := -Wall -Werror -g
LDFLAGS := 


OBJECTS :=  main.o shell.o

all: w4118_sh test_w4118_sh

w4118_sh: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

test_w4118_sh: shell.o test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ shell.o test.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f w4118_sh
	rm -f test_w4118_sh
	rm -f shell.o
	rm -f test.o
	rm -f main.o

.PHONY: clean
