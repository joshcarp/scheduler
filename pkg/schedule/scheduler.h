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

int next(node_t *head, enum scheduler type, int quantum, int time);

int apply_quantum (datat *next, int quantum, int time);

int next_helper (node_t *head, enum scheduler type, int quantum, int time);

int left(node_t *head);