/* Header File for algorithm.c 
   rokane - 587723 - Ryan O'Kane */

void load(meminfo_t *memory, process_t *to_load, process_t *queue, char *algorithm);
int scan_list(process_t *free_list, process_t *to_load, char* algorithm, meminfo_t *memory);
void update_free_list(int start, int size, process_t *free_list, meminfo_t *memory);
void remove_largest(process_t *processes, process_t *free_list, meminfo_t *memory, process_t *queue);
void add_memory(meminfo_t *memory, int start, int size);








