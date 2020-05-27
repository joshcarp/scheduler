#ifndef BST_H
#define BST_H

#include "../bst/bst.h"
#endif

#ifndef DATA_H
#define DATA_H

#include "../bst/data.h"
#endif

enum scheduler {
    first_come = 0,
    round_robin = 1,
} scheduler;

int next(datat *head, enum scheduler type, int quantum);

int apply_quantum (datat *head, datat *next, int quantum, int time);

int next_helper (datat *head, enum scheduler type, int quantum, int time);

int left(datat *head);