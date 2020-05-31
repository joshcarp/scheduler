#ifndef BST_H
#define BST_H
#endif

#ifndef DATA_H
#define DATA_H

#include "structures.h"
#endif

enum scheduler_algorithms {
    unset_scheduler = 0,
    first_come = 1,
    round_robin = 2,
    custom_schedule = 3,
} scheduler_algorithms;

enum memory_algorithm {
    unset_memory = 0,
    unlimited = 1,
    swapping = 2,
    virtual = 3,
    custom_memory = 4,

} memory_algorithm;

/* "Main" functions that do most of the heavy lifting */
int run (process *head, int quantum, int memory_size, enum memory_algorithm mem_algo, enum scheduler_algorithms schedule);

int apply_quantum (mem *memory, process *head, process *next, int quantum, int time, int loadtime, enum scheduler_algorithms type);

int assign_memory (mem *memory, queue *q, process *next, int time, enum memory_algorithm type);

int left (process *head, int time);


/* Printing functions */
void print_stats (process *head, int time);

void print_addresses (page **arr, int n, bool allocated);

void print_evicted (mem *memory, int time);

/* Memory management functions */

bool memoryallocate (mem *memory, page *p);

bool evict_process (mem *memory, process *next);

bool evict_page (mem *memory, page *next);

int loaded_pages (mem *memory);

