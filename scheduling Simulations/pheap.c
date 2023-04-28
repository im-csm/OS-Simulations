#include"pa1.h"

//creates an empty heap of given cap
pheap create_pheap(unsigned capacity){
    pheap h;
    h.processes = (process*)malloc((1 + capacity)*sizeof(process));
    h.size = 0;
    h.capacity = capacity;
    return h;
}

pheap pheap_clone(pheap h){
    pheap copy = h;
    copy.processes = (process*)malloc((1 + h.capacity)*sizeof(process));
    for(int i = 0; i < h.capacity+1; i++)
        copy.processes[i] = h.processes[i];
    return copy;
}

int pheap_is_empty(pheap h)
{
    return h.size == 0;
}

int pheap_is_full(pheap h){
    return h.size == h.capacity;
}

void clear_pheap(pheap* h){
    h->size = 0;
}

//heapify or max-percolate-down is essential for delete root operation
void pheap_max_percolate_down(pheap h, int node){//log(n) operation
    //fixes the max-heap property violated by node!
    while(1){
        int left_child = node << 1;
        int right_child = left_child + 1;
        int max = node;
        if(left_child <= h.size && h.processes[left_child].burst_time > h.processes[node].burst_time)
            max = left_child;
        if(right_child <=h.size && h.processes[right_child].burst_time > h.processes[max].burst_time)
            max = right_child;
        if(max != node){    //swap the node with its largest child
            process temp = h.processes[node];
            h.processes[node] = h.processes[max];
            h.processes[max] = temp;
            node = max;
        }else   //end of story!
            break;
    }
}

//insert function adds an item to a given max_heap passed by reference
//O(log n) operation that uses percolate-up to preserve max-heap property
void pheap_max_insert(pheap *h, process item){
    if(h->size == h->capacity)
        return;
    printf("Process with bt = %d enqueued to max queue\n", item.burst_time);
    int index = ++h->size;
    h->processes[index] = item;
    while(index != 1){//max percolate-up
        int parent = index >> 1;
        if(h->processes[parent].burst_time > h->processes[index].burst_time)
            return;
        process temp = h->processes[parent];
        h->processes[parent] = h->processes[index];
        h->processes[index] = temp;
        index = parent;
    }
}

//max_delete function removes the root of a max_heap
//O(log n) operation that uses percolate-down to preserve max-heap property
process pheap_max_delete(pheap *h){
    process rv = h->processes[1];//root
    printf("Process with bt = %d dequeued from max priority queue\n", rv.burst_time);
    h->processes[1] = h->processes[h->size--];
    pheap_max_percolate_down(*h, 1);
    return rv;
}

process pheap_get_max(pheap h){
    return h.processes[h.capacity];
}

//heapify or min-percolate-down is essential for delete root operation
void pheap_min_percolate_down(pheap h, int node){//log(n) operation
    //fixes the min-heap property violated by node!
    while(1){
        int left_child = node << 1;
        int right_child = left_child + 1;
        int min = node;
        if(left_child <= h.size && h.processes[left_child].burst_time < h.processes[node].burst_time)
            min = left_child;
        if(right_child <=h.size && h.processes[right_child].burst_time < h.processes[min].burst_time)
            min = right_child;
        if(min != node){//swap the node with its smallest child
            process temp = h.processes[node];
            h.processes[node] = h.processes[min];
            h.processes[min] = temp;
            node = min;
        }else//end of story!
            break;
    }
}

//insert function adds an item to a given min_heap passed by reference
//O(log n) operation that uses percolate-up to preserve min-heap property
void pheap_min_insert(pheap *h, process item){
    if(h->size == h->capacity)
        return;
    //printf("Process with bt = %d enqueued to min queue\n", item.burst_time);
    int index = ++h->size;
    h->processes[index] = item;
    while(index != 1){//min percolate-up
        int parent = index >> 1;
        if(h->processes[parent].burst_time < h->processes[index].burst_time)
            return;
        process temp = h->processes[parent];
        h->processes[parent] = h->processes[index];
        h->processes[index] = temp;
        index = parent;
    }
}

//min_delete function removes the root of a min_heap
//O(log n) operation that uses percolate-down to preserve min-heap property
process pheap_min_delete(pheap *h){
    process rv = h->processes[1];//root
    //printf("%d dequeued from min priority queue\n", rv);
    h->processes[1] = h->processes[h->size--];
    pheap_min_percolate_down(*h, 1);
    return rv;
}

process pheap_get_min(pheap h){
    return h.processes[1];
}
