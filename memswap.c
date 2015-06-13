
/* 
   Memory simulation program. 
   Simulates the swapping in and out of processes
   that the memory management program in the operating system does.
   Can perform simulation process according to 4 different algorithms:
    - First: Puts process in first available hole in memory
    - Best: Puts process in hole in memory with smallest leftover gap
    - Worst: Puts process in hole in memory with largest leftover gap
    - Next: Puts process in next available hole starting from previous position
   Takes input under options: -f "filename", -m "memory", -a "algorithm".
   Written by Ryan O'Kane - 587723 - rokane
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "queue.h"
#include "memswap.h"
#include "memory.h"
#include "algorithm.h"

#define DEBUGQUEUE 0

/* Extern declarations */
extern int optind;
extern char *optarg;

int main(int argc, char *argv[])
{	
	/* Queue which will store processes waiting to be loaded to memory */
	process_t *queue = safe_malloc(sizeof(process_t));

	char input;
	char *algorithm = "first";		/* Initialize to First algorithm */
	char *file;
	int max_memory;

	/* Read input arguments corresponding to their particular options */
	while((input = getopt(argc, argv, "f:a:m:")) != EOF)
	{
		switch(input)
		{
			/* Filename option */
			case 'f':
				file = optarg;
				break;

			/* Algorithm option */
			case 'a':
				if(strcmp(optarg, "next") == 0)
					algorithm = optarg;

				else if(strcmp(optarg, "best") == 0)
					algorithm = optarg;

				else if(strcmp(optarg, "worst") == 0)
					algorithm = optarg;

				else if (strcmp(optarg, "first") != 0)
				{
					fprintf(stderr, "Unknown option for algorithm: %s \n", optarg);
					exit(1);
				}
				break;

			/* Memory size option */
			case 'm':
				max_memory = atoi(optarg);
				break;

			default:
				break;
		}
	} 

	queue = read_processes(file, queue);

	/* Initialize memory */
	meminfo_t *memory = safe_malloc(sizeof(meminfo_t));
	memory = initialize_memory(max_memory, memory);

	process_t *loaded;

	/* Continues until there are no more processes waiting to be loaded */
	while(!empty_queue(queue))
	{
		/* Get next process off the top of the queue and load into memory */
		loaded = pop(&queue);
		load(memory, loaded, queue, algorithm);
	}

	return 0;
}

/* Debugging function which prints the contentes of a queue */
void print_queue(process_t *queue)
{
	process_t *current = queue;

	while(current != NULL)
	{
		printf("PID - %d, MEMORY - %d, START - %d, INDEX - %d \n", current->PID, 
			current->size, current->start_index, current->index_flag);

		current = current->next;

	}
} 




