#rokane - 587723 - Ryan O'Kane

memswap:     queue.o memswap.o memory.o algorithm.o
	     gcc -o memswap queue.o memswap.o memory.o algorithm.o -lm

queue.o:     queue.c queue.h
	     gcc -c -Wall queue.c

driver.o:   driver.h
	     gcc -c -Wall driver.c
		   
memory.o:   memory.h
	     gcc -c -Wall memory.c

algorithm.o:   algorithm.h
	     gcc -c -Wall algorithm.c