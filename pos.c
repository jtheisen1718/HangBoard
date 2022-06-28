#include <stdio.h>
#include <stdlib.h>
#include "pos.h"

pos make_pos(unsigned int r, unsigned int c){
    pos a = {r,c};
    return a;
}

posqueue* posqueue_new(){
    posqueue* a = (posqueue*)malloc(sizeof(pq_entry)*2+sizeof(unsigned int));
    if (a==NULL){
        fprintf(stderr,"Unable to allocate memory\n");
        exit(1);
    }
    a->len = 0;
    a->head = NULL;
    a->tail = NULL;
    return a;
}

pq_entry* free_pq_entry(pq_entry* e){
    pq_entry* n = e->next;
    free(e);
    return n;
}

void pos_enqueue(posqueue* q, pos p){
    pq_entry* e = (pq_entry*)malloc(sizeof(pq_entry));
    if (e==NULL){
        fprintf(stderr,"Unable to allocate memory\n");
        exit(1);
    }
    e->p = p;
    if (q->len != 0){
        q->tail->next = e;
    } else {
        q->head = e;
    }
    q->tail = e;
    q->len++;
}

pos pos_dequeue(posqueue* q){
    if (q->len == 0){
        fprintf(stderr,"pos_dequeue: Invalid input (q->len): 0\n");
        exit(1);
    }
    pos p = q->head->p;
    q->head = free_pq_entry(q->head);
    q->len--;
    if (q->len == 0){
        q->tail = NULL;
    }
    return p;
}

bool pos_equals(pos a, pos b){
    return (a.c == b.c && a.r == b.r);
}

bool posqueue_member(posqueue* q, pos p){
    if (q->len != 0){
        pq_entry* curr = q->head;
        while (curr){
            if (pos_equals(curr->p,p)){
                return true;
            }
            curr = curr->next;
        }
    }
    return false;
}

void posqueue_free(posqueue* q){
    if (q != NULL){
        pq_entry* curr = q->head;
        while (curr){
            curr = free_pq_entry(curr);
        }
        free(q);
    }
}
