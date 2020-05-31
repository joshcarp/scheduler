#ifndef BST_H
#define BST_H

#include "../bst/bst.h"
#endif

#ifndef DATA_H
#define DATA_H

#include "../bst/data.h"
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

int run (datat *head, int quantum, int memory_size, enum memory_algorithm malgo, enum scheduler_algorithms schedule);

int apply_quantum (mem *memory, datat *head, datat *next, int quantum, int time, int loadtime, enum scheduler_algorithms type);

int left (datat *head, int time);

int assign_memory (mem *memory, queue *q, datat *next, int quantum, int time, enum memory_algorithm type);
