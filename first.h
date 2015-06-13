

void first_load(meminfo_t *memory, process_t *to_load);
int scan_list(process_t *free_list, process_t *to_load, char* algorithm);
void update_free_list(int start, int size, process_t *free_list);










