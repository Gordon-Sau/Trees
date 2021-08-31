CC := gcc

all: testAVL

testAVL: testAVL.c AVLTree.o
	$(CC) -o testAVL testAVL.c AVLTree.o -g -Wall

AVLTree.o: AVLTree.c AVLTree.h
	$(CC) -c AVLTree.c -g -Wall

clean:
	rm *.o testAVL