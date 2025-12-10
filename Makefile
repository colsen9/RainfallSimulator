mapper: mapper.o data.o
	gcc -Wall -g -o mapper mapper.o data.o

mapper.o: mapper.c data.h
	gcc -Wall -g -c mapper.c

data.o: data.c data.h
	gcc -Wall -g -c data.c

clean:
	rm -f mapper *.o
