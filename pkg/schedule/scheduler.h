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
int run (process *head, int quantum, uint memory_size, enum memory_algorithm mem_algo, enum scheduler_algorithms schedule);

int apply_quantum (mem *memory, process *head, process *next, uint quantum, uint time, enum scheduler_algorithms type);

uint assign_memory (mem *memory, queue *q, process *next, uint time, uint (*evict) (mem *, process *, uint));

uint left (process *head, uint time);


/* Printing functions */
void print_stats (process *head, int time);

void print_addresses (page **arr, uint n, bool allocated);

void print_evicted (mem *memory, int time);

/* Memory management functions */

bool memoryallocate (mem *memory, page *p);

bool evict_process (mem *memory, process *next);

bool evict_page (mem *memory, page *next);

int loaded_pages (mem *memory);

uint virtual_memory_evict (mem *memory, process *head, uint needed_pages);

uint swapping_memory_evict (mem *memory, process *head, uint needed_pages);

uint custom_memory_evict (mem *memory, process *head, uint needed_pages);

int evict_upto (mem *memory, mem *to_evict, int needed_pages);



/* Math functions because the environment i was working on didn't support math.h */

float ceiling (float x);

float round_5 (float x);
