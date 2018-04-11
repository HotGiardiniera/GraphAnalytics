CFLAGS=-Wall -Wpedantic
CC=g++

bfs:
	$(CC) $(CFLAGS) bfs.cpp Vertex.cpp -o bfs

pbfs:
	$(CC) $(CFLAGS) -fopenmp pbfs.cpp Vertex.cpp -o pbfs

topsort:
	$(CC) $(CFLAGS) topsort.cpp Vertex.cpp -o topsort

clean:
	rm -rf bfs; rm -rf topsort;