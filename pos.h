#ifndef POS_H
#define POS_H

#include <stdbool.h>

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;


typedef struct pq_entry pq_entry;

struct pq_entry {
    pos p;
    pq_entry* next;
};


struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;


/* pos constructor */
pos make_pos(unsigned int r, unsigned int c);

/* allocate a new, empty queue */
posqueue* posqueue_new();

/* entirely deallocate a pq_entry 
   and return a pointer to the pq_entry pointed to by e->next */
pq_entry* free_pq_entry(pq_entry* e);

/* add a position to the queue, at the back */
void pos_enqueue(posqueue* q, pos p);

/* remove and return the element at the front of the queue 
   or raise an error if the queue is empty */
pos pos_dequeue(posqueue* q);

/* returns true if the two positions are the same */
bool pos_equals(pos a, pos b);

/* returns true if the position is in the queue, and false otherwise */
bool posqueue_member(posqueue* q, pos p);

/* entirely deallocate an existing queue */
void posqueue_free(posqueue* q);

#endif /* POS_H */
