
/* Header file for queue.c 
   rokane - 587723 - Ryan O'Kane */

/* Contains information related to an individual process */
typedef struct process_t
{
	int PID;					/* Process ID */
	int size;					/* Total size of process */
	int run_time;				/* How many times it has been in memory */
	int index_flag;				/* Flag to indicate where to scan from in next algorithm */
	struct process_t *next;		/* Pointer to next process in list */
	int start_index;			/* Start point in memory */
}process_t;


void* safe_malloc(int size);
process_t* read_processes(char *file, process_t *start);
void insert_process(process_t *process, int id, int size, int run, int mem_time, int start);
void push(process_t *queue, process_t *new_process);
process_t* pop(process_t **queue);
int empty_queue(process_t *queue);
   






















