# COMP30023-Memory Simulation Program 

Program was written for a third year subject at the University of Melbourne (Computer Systems - COMP30023).
The program is designed to simulate the way memory is allocated by the Operating Systems according to different alocation algorithms. It shows the number of used space, available space, and holes in memory as the processes are swapped in and out of main memory. 
This program does not optimize memory alocation, simply just a simulation.


Makefile will create executable program call 'memswap'. Simply type "make".

Usage:
To run the program, type the executable name (memswap) followed by the following required options:

    -f 'filename'. Filename containing proccess IDs and corresponding sizes. Sample input file included.
    -a 'algorithm'. Must be one of {first, best, next, worst}.
    -m 'memory size'. Total capacity of memory.
    

Example Usage:
    ./memswap -f test_programs -a first -m 200
    
    
    










