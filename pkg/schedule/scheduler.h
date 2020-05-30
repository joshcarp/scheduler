#ifndef BST_H
#define BST_H

#include "../bst/bst.h"
#endif

#ifndef DATA_H
#define DATA_H

#include "../bst/data.h"
#endif

enum scheduler {
    unset_scheduler = 0,
    first_come = 1,
    round_robin = 2,
    custom_schedule = 3,
} scheduler;

enum memory_algorithm {
    unset_memory = 0,
    unlimited = 1,
    swapping = 2,
    virtual = 3,
    custom_memory = 4,

} memory_algorithm;

int next (datat *head, enum scheduler type, int quantum, int memory_size, enum memory_algorithm malgo, enum scheduler schedule);

int apply_quantum (mem *memory, datat *head, datat *next, int quantum, int time, enum scheduler type);

int next_helper (datat *head, enum scheduler type, int quantum, int memory_size,int time, enum memory_algorithm malgo, enum scheduler schedule);

int left(datat *head);

int assign_memory (mem *memory, queue *q, datat *next, int quantum, int time, enum memory_algorithm type);
