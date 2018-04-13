CFLAGS=-Wall -Wpedantic
CC=g++

pagerank:
	$(CC) $(CFLAGS) pagerank.cpp PageNode.cpp -o pagerank

ppagerank:
	$(CC) $(CFLAGS) -fopenmp ppagerank.cpp PageNode.cpp -o ppagerank

clean:
	rm -rf pagerank; rm -rf ppagerank;