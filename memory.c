/* Contains functions related to the processing of memory 
   rokane - 587723 - Ryan O'Kane */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "queue.h"
#include "memswap.h"
#include "memory.h"
#include "algorithm.h"

/* Initialises housekeeping into for memory and allocates space for lists */
meminfo_t* initialize_memory(int size, meminfo_t *memory)
{
	memory->total_size = size;
	memory->used_memory = 0;
	memory->holes = 1;				
	memory->num_processes = 0;

	/* Allocate memory for free space */
	memory->free_mem = safe_malloc(sizeof(process_t));
	/* Index = 1 as this is where next should start */
	insert_process(memory->free_mem, 0, size, 0, 1, 0);

	/* Allocate memory for process list */
	memory->processes = safe_malloc(sizeof(process_t));
	insert_process(memory->processes, 0, 0, 0, 0, 0);

	return memory;
}

/* Updates housekeeping information of overall memory and prints status to stdout */
void update_housekeeping(meminfo_t *memory, process_t *loaded)
{
	int num_holes = 0;
	int num_processes = 0;
	int used_mem = 0;
	int mem_usage;
	process_t *free_list = memory->free_mem;
	process_t *process_list = memory->processes;

	/* Count holes in free_list */
	while(free_list != NULL)
	{
		if(free_list->size > 0)
			num_holes++;

		free_list = free_list->next;
	}
	/* Count processes running in memory, and add up total used memory */
	while(process_list != NULL)
	{
		num_processes++;
		used_mem += process_list->size;
		process_list = process_list->next;
	}

	memory->num_processes = num_processes;
	memory->holes = num_holes;
	memory->used_memory = used_mem;

	/* Convert to percentage value */
	mem_usage = ceil(((double)memory->used_memory/(double)memory->total_size)*100);

	printf("%d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
		loaded->PID, memory->num_processes, memory->holes, mem_usage);

}































