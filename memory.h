
/* Header file for memory.c 
   rokane - 587723 - Ryan O'Kane */

/* Contains information realting to overall memory */
typedef struct meminfo_t
{
	int total_size;						/* Total capacity of memory */	
	int used_memory;					/* Amount of memory used by processes */
	int holes;							/* Number of free positions in memory */
	int num_processes;					/* Number of processes currently in memory */
	struct process_t* processes;		/* Pointer to list of processes in memory */
	struct process_t* free_mem;			/* Pointer to list of free memory */
}meminfo_t;

meminfo_t* initialize_memory(int size, meminfo_t* memory);
void update_housekeeping(meminfo_t *memory, struct process_t *loaded);























