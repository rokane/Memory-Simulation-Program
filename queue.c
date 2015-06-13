/* Contains functions related to the processing of queues 
   rokane - 587723 - Ryan O'Kane */

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


#define MAXLINE 10
#define DEBUGLINE 0

/* Function reads input data from a file, saves data as a process, 
   and returns the queue with all proceses waiting to be loaded into memory */
process_t *read_processes(char *file, process_t *queue)
{
	int process_num;
	int process_size;
	int first_read = 1;
	FILE *fp;
	char line[MAXLINE];     

	/* Open file for reading, check if file is empty */
	if( (fp=fopen(file,"r")) ==NULL)
	{
		printf("Error file %s is empty.\n", file);
		exit(1);
	}

	/* Read from file  */
	while(fgets(line, sizeof(line), fp))
	{
		sscanf(line, "%d %d", &process_num, &process_size);

		if(DEBUGLINE)
			printf("Line Read: PID - %d, MEMORY - %d \n", process_num, process_size);

		if(first_read)
		{
			/* If its the first line, put data at start of queue */
			insert_process(queue, process_num, process_size, 0, 0, 0);
			first_read = 0;
			continue;
		}

		/* Allocate memory for new process and push onto the queue */
		process_t *process_read = safe_malloc(sizeof(process_t));
		insert_process(process_read, process_num, process_size, 0, 0, 0);
		push(queue, process_read);

	}

	return queue;

}

/* Allocates memory to be used, prints an error if something goes wrong */
void* safe_malloc(int size)
{
	void *a = malloc(size);
	
	if(a == NULL){
	
		printf("Malloc error\n");
		exit(1);
	}
	
	return a;

}

/* Inserts corresponding data into a process */
void insert_process(process_t *process, int id, int size, int run, int flag, int start)
{
	process->PID = id;
	process->size = size;
	process->run_time = run;
	process->index_flag = flag;
	process->next = NULL;	
	process->start_index = start;
}

/* Pushes process onto the end of the queue */
void push(process_t *queue, process_t *new_process)
{
	/* If next is empty, link */
	if(queue->next == NULL){
		queue->next = new_process;
	}
	
	/* Try again with next spot in list */
	else
		push(queue->next, new_process);
	
}

/* Removes the next process from the list and returns it */
process_t* pop(process_t **queue)
{
	process_t *start_process = safe_malloc(sizeof(process_t));
	process_t *next_node = NULL;

	/* Update pointers so the start of the queue points to correct position */
	next_node = (*queue)->next;
	insert_process(start_process, (*queue)->PID, (*queue)->size, (*queue)->run_time, 
		(*queue)->index_flag, 0);

	free(*queue);
	*queue = next_node;

	return start_process;
}

/* Returns 1 if the queue if empty, 0 otherwise */
int empty_queue(process_t *queue)
{
	int empty;

	if(queue == NULL)
		empty = 1;
	else
		empty = 0;

	return empty;
}











