#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "queue.h"
#include "memswap.h"
#include "first.h"



void first_load(meminfo_t *memory, process_t *to_load)
{
	/* Scan free list and return first hole of fit */
	int start_pos;
	start_pos = scan_list(memory->free_mem, to_load, "First"); // Need to change First to a valid constant.
	to_load->start_index = start_pos;
	to_load->run_time++;



	/* Push process into memory->processes */
	push(memory->processes, to_load);

	/* PROBLEM HERE WHEN I ACCESS to_load it seg faults, could be the reason why im getting memsize 200 instead of 90 for particular process */
	/* Update free list  */
	update_free_list(to_load->start_index, to_load->size, memory->free_mem);

	printf("PROCESS LIST: \n");
	print_queue(memory->processes);
	printf("\nFREE MEMORY LIST: \n");
	print_queue(memory->free_mem);


}

/* Returns start position of the desired hole */
int scan_list(process_t *free_list, process_t *to_load, char* algorithm)
{
	int start_point;
	if(strcmp(algorithm, "First") == 0){

		while(free_list != NULL)
			{
				if(free_list->size > to_load->size){
					start_point = free_list->start_index;

					return start_point;
				}
				else
					free_list = free_list->next;
			}
	}

	else if(strcmp(algorithm, "Next") == 0){


	}

	else if(strcmp(algorithm, "Best") == 0){


	}

	else if(strcmp(algorithm, "Worst") == 0){


	}

}

void update_free_list(int start, int size, process_t *free_list)
{
	process_t *previous_hole;

	// Find position that needs to be changed
	while(free_list->start_index != start)
	{
		previous_hole = free_list;
		free_list = free_list->next;
	}

	// Update size of the hole
	free_list->size = free_list->size - size;
	free_list->start_index = free_list->start_index + size; 

	// Check adjacent holes 
	// Check previous hole
	if(previous_hole->start_index + previous_hole->size == free_list->start_index)
	{
		// Merge
		previous_hole->size = previous_hole->size + free_list->size;
		previous_hole->next = free_list->next;
		free(free_list);
	}

	// Check following holes
	if((free_list->next != NULL) && (free_list->start_index + free_list->size == free_list->next->start_index))
	{
		// Merge
		free_list->size = free_list->size + free_list->next->size;
		free(free_list->next);
	}

}



















