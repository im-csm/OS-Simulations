#include<stdio.h>
#include<stdlib.h>
#include <limits.h>

#ifndef PA1_PA1_H
#define PA1_PA1_H

typedef struct{
    int arrival_time;
    int burst_time;
    int priority_level;
    int remaining_time;
    int tt;
} process;

typedef struct node{
    struct node* next;
    process value;
} node;

typedef struct queue2{
    node* head;
    node* tail;
    int size;
} lnklst_queue;

typedef struct heap{
    int* data;
    int size;
    unsigned capacity;
} heap;

typedef struct pheap{
    process* processes;
    int size;
    unsigned capacity;
} pheap;

typedef struct{
    int front, rear, size;
    unsigned capacity;
    process* array;
}circ_arr_queue;
#endif

//circular array queue
circ_arr_queue create_queue1(unsigned);
int is_full1(circ_arr_queue);
int is_empty1(circ_arr_queue);
void enqueue1(circ_arr_queue*, process);
process dequeue1(circ_arr_queue*);
process front1(circ_arr_queue);
process rear1(circ_arr_queue);

// Linked list queue
lnklst_queue create_queue2();
int is_full2(lnklst_queue);
int is_empty2(lnklst_queue);
void enqueue2(lnklst_queue*, process);
process dequeue2(lnklst_queue*);
process front2(lnklst_queue);
process rear2(lnklst_queue);

// Processes heap
pheap create_pheap(unsigned);
pheap pheap_clone(pheap h);
int pheap_is_empty(pheap);
int pheap_is_full(pheap);
void clear_pheap(pheap*);
void pheap_max_insert(pheap*, process item);
process pheap_max_delete(pheap*);
process pheap_get_max(pheap);
void pheap_min_insert(pheap*, process item);
process pheap_min_delete(pheap*);
process pheap_get_min(pheap);

// Max or Min heap
heap create_heap(unsigned);
heap clone(heap h);
int is_empty(heap);
int is_full(heap);
void clear(heap*);
void max_insert(heap*, int);
int max_delete(heap*);
int get_max(heap);
void min_insert(heap*, int);
int min_delete(heap*);
int get_min(heap);

