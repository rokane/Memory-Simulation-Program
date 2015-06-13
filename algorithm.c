/* Contains functions related to the memory management algorithms 
   rokane - 587723 - Ryan O'Kane */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "memory.h"
#include "queue.h"
#include "memswap.h"
#include "algorithm.h"

#define DEBUGLOAD 0
#define DEBUGREMOVE 0
#define DEBUGINFO 0

/* Loads a process into memory, if there is not enough free space 
   in memory, it will remove the largest process and try again */
void load(meminfo_t *memory, process_t *to_load, process_t *queue, char *algorithm)
{
	int start_pos;
	int size;

	/* First process in the list */
	if(memory->processes->PID == 0)
	{
		/* Update process in memory->processes to = loading process */
		start_pos = 0;
		size = to_load->size;
		insert_process(memory->processes, to_load->PID, to_load->size,
			 ++to_load->run_time, 0, start_pos);

	}
	/* Scan free list and return first hole of fit */
	else
	{	
		/* If no hole large enough was found */
		if((start_pos=scan_list(memory->free_mem, to_load, algorithm, memory))<0){
			
			/* Remove largest process from memory */
			remove_largest(memory->processes, memory->free_mem, memory, queue);

			/* recursivly repeat */
			load(memory, to_load, queue, algorithm);
			return;
		}	
		/* Update start position of the process */
		else{
			start_pos = scan_list(memory->free_mem, to_load, algorithm, memory); 
			size = to_load->size;
			insert_process(to_load, to_load->PID, to_load->size, ++to_load->run_time,
				 0, start_pos);
		}
		

		/* Push process into memory->processes */
		push(memory->processes, to_load);			

	}

	/* Update free list  */
	update_free_list(start_pos, size, memory->free_mem, memory);

	/* Update housekeeping info and print to stdout */
	update_housekeeping(memory, to_load);

	if(DEBUGINFO){
		printf("\nQueue Status\n");
		print_queue(queue);
		printf("Free List Status\n");
		print_queue(memory->free_mem);
		printf("Process Status\n");
		print_queue(memory->processes);
	}


}

/* Returns start position of the hole in memory according to particular
   algorithm. If no hole was big enough to hold process, return -1 */
int scan_list(process_t *free_list, process_t *to_load, char* algorithm, meminfo_t *memory)
{
	int start_point = -1;
	int gap;
	int smallest_gap = INT_MAX;
	int largest_gap = -1;

	/* First Algorithm */
	if(strcmp(algorithm, "first") == 0){

		/* Scan free list until you reach the end */
		while(free_list != NULL)
			{
				/* If a hole is large enough, update and return start point */
				if(free_list->size >= to_load->size){
					start_point = free_list->start_index;

					return start_point;
				}

				else
					free_list = free_list->next;
			}
			return start_point;
	}

	/* Next Algorithm */
	else if(strcmp(algorithm, "next") == 0){

		int index_start;

		/* Scan through list until you reach indexed position */
		while(free_list->index_flag != 1)
		{
			free_list = free_list->next;
		}

		/* Keep track of initial start_index */
		index_start = free_list->start_index;

		/* Scan through list from indexed position until you get back to 
		   indexed position */
		do{
			/* If a hole is large enough, update and return start point */
			if(free_list->size >= to_load->size){
				start_point = free_list->start_index;

				return start_point;
			}
			/* Move to next hole */
			else{
				free_list->index_flag = 0;		/* Update index info */

				/* If it is the end of the list, look at the start */
				if(free_list->next != NULL)		
					free_list = free_list->next;
				else
					free_list = memory->free_mem;

				free_list->index_flag = 1;		/* Update index info */
			}		

		}while(free_list->start_index != index_start);

		return start_point;
	}

	/* Best Algorithm */
	else if(strcmp(algorithm, "best") == 0){

		/* Scan free list until you reach the end */
		while(free_list != NULL)
		{
			/* If a hole is large enough check the remaining memory */
			if(free_list->size >= to_load->size){

				gap = free_list->size - to_load->size;

				/* If its smaller than prev best, update start point */
				if(gap < smallest_gap){

					smallest_gap = gap;
					start_point = free_list->start_index;
				}
			}
			free_list = free_list->next;
		}
		
	}

	/* Worst Algorithm */
	else if(strcmp(algorithm, "worst") == 0){

		/* Scan free list until you reach the end */
		while(free_list != NULL)
		{
			/* If a hole is large enough check the remaining memory */
			if(free_list->size >= to_load->size){

				gap = free_list->size - to_load->size;

				/* If it is larger than before, update start point */
				if(gap > largest_gap){

					largest_gap = gap;
					start_point = free_list->start_index;
				}
			}
			free_list = free_list->next;
		}
		
	}
	return start_point;
}

/* Updates the free list information after a process is loaded into memory */
void update_free_list(int start, int size, process_t *free_list, meminfo_t *memory)
{
	process_t *previous_hole = NULL;

	// Find position that needs to be changed
	while(free_list->start_index != start)
	{
		previous_hole = free_list;			/* Keep track of preceding node */
		free_list = free_list->next;
	}

	// Update start & size of the hole
	free_list->size = free_list->size - size;
	free_list->start_index = free_list->start_index + size; 
	
	/* Check for a perfect fit and remove node if there is, unless it is the last node */
	if((free_list->size == 0) && (memory->holes > 1)){

		/* If it is the start hole in the list */
		if(previous_hole == NULL){
			memory->free_mem = free_list->next;		/* Update memory pointer */

			if(free_list->index_flag == 1)
				memory->free_mem->index_flag = 1;	/* Update index position */
		}
		/* Otherwise channge previous hole pointer */
		else{
			previous_hole->next = free_list->next;

			if(free_list->index_flag == 1)
				previous_hole->index_flag = 1;		/* Update index position */
		}
		free(free_list);
	}


	/* Check adjacent holes to see if they need to be merged into one */
	/* If the previous hole end point == current start point, merge */
	if((previous_hole != NULL) && (previous_hole->start_index + 
				previous_hole->size == free_list->start_index))
	{
		/* Current hole joins previous hole */
		previous_hole->size = previous_hole->size + free_list->size;
		previous_hole->next = free_list->next;

		if(free_list->index_flag == 1)
			previous_hole->index_flag = 1;		/* Update index info */

		free(free_list);
	}
	/* If following hole start point = current end point, merge */
	if((free_list->next != NULL) && (free_list->start_index + 
				free_list->size == free_list->next->start_index))
	{
		/* Following hole joins current hole */
		free_list->size = free_list->size + free_list->next->size;
		free(free_list->next);
	}

}

/* Removes the largest running process from memory */
void remove_largest(process_t *processes, process_t *free_list, meminfo_t *memory, 
		process_t *queue)
{
	process_t *prev_index;
	process_t *largest_prev;
	process_t *current_largest;
	int largest = 0;
	int first_load = 0;
	int initial_load_flag = 0;

	/* Loop through process list and keep track of the largest */
	while(processes != NULL)
	{
		/* Update largest process if it is larger */
		if(processes->size > largest){
			largest = processes->size;
			current_largest = processes;

			if(first_load == 0){
				initial_load_flag = 1;		/* Keep track if largest is the start of list */
				
			}
			else{
				initial_load_flag = 0;
				largest_prev = prev_index;	/* Keep track of preceding node */

			}

			first_load++;
		}
		prev_index = processes;
		processes = processes->next;
	}

	/* If largest was first in list, need to update memory->processes pointer */
	if(initial_load_flag){
		memory->processes = current_largest->next;
	}
	/* else update previous node pointer */
	else{
		largest_prev->next = current_largest->next;
	}

	/* Add more memory to the free list */
	add_memory(memory, current_largest->start_index, current_largest->size);


	/* If largest process->run_time = 3, it has finished its cycle */
	if(current_largest->run_time == 3)
		free(current_largest);

	/* Otherwise push it onto the queue for further processing */
	else{
		insert_process(current_largest, current_largest->PID, current_largest->size, 
			current_largest->run_time, 0, 0);
		push(queue, current_largest);
	}
}

/* Adds memory to the free list when a process is removed from memory */
void add_memory(meminfo_t *memory, int start, int size)
{
	process_t *current = memory->free_mem;
	process_t *previous;
	process_t *next;
	int first_flag = 1;

	/* Scan through all holes in free list */
	while(current != NULL)
	{
		/* Check if new memory need to be merged with other holes */
		if(current->start_index + current->size == start){
			current->size = current->size + size;

			/* Check following cell and merge if neccesary */
			if((current->next != NULL) && (start + size == current->next->start_index)){
				/* Merge holes together */
				next = current->next;

				/* Update index flag */
				if(current->next->index_flag)
					current->index_flag = 1;
				
				current->size = current->size + next->size;
				
				free(current->next);
				current->next = next->next;
				
			}
			return;
		}
		/* Update size and start index of hole */
		if(start + size == current->start_index){
			current->size = current->size + size;
			current->start_index = start;

			return;
		}
		previous = current;
		current = current->next;
	}

	/* If we get to here we need to make a new hole of memory */
	process_t *new_hole = safe_malloc(sizeof(process_t));
	insert_process(new_hole, 0, size, 0, 0, start);

	/* Make current point to start of free list */
	current = memory->free_mem;
	
	/* Put memory in appropriate position, ordered by start_index */
	while(current != NULL)
	{
		if(new_hole->start_index < current->start_index){

			if(first_flag){
				memory->free_mem = new_hole;
				new_hole->next = current;

				return;
			}
			else{
				previous->next = new_hole;
				new_hole->next = current;

				return;
			}
		}
		--first_flag;
		previous = current;
		current = current->next;
	}
	/* If we get to here, hole goes on the end of the list */
	push(memory->free_mem, new_hole);

}

