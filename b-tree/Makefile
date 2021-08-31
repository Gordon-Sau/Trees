CC:=gcc
flags:=-Wall -Werror -g

all: test_btree

test_btree: test_btree.c 2_3_4tree.o Stack.o
	$(CC) $(flags) -o test_btree test_btree.c 2_3_4tree.o Stack.o

2_3_4tree.o: 2_3_4tree.c 2_3_4tree.h
	$(CC) $(flags) -c 2_3_4tree.c

Stack.o: Stack.c Stack.h
	$(CC) $(flags) -c Stack.c

clean:
	rm *.o test_btree