all: encrypt-module.o header.o
	gcc -pthread encrypt-module.o -o encrypt352 -lm

encrypt-module.o: encrypt-module.c 
	gcc -c encrypt-module.c

header.o: encrypt-module.h
	gcc -c encrypt-module.h

clean:
	rm *.o *.h.gch encrypt352