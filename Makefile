# Part A
mapper: mapper.o data.o
	gcc -Wall -g -o mapper mapper.o data.o

mapper.o: mapper.c data.h
	gcc -Wall -g -c mapper.c

data.o: data.c data.h
	gcc -Wall -g -c data.c

clean:
	rm -f mapper *.o

# Part B
testgraph: testgraph.o graph.o data.o
	gcc -Wall -g -o testgraph testgraph.o graph.o data.o

testgraph.o: testgraph.c testgraph.h graph.h data.h
	gcc -Wall -g -c testgraph.c

graph.o: graph.c graph.h
	gcc -Wall -g -c graph.c

clean:
	rm -f mapper testgraph *.o