#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include<time.h>
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))

double avg_utilization = 0;
double number_of_examined_holes = 0;
int head = 0;//head of the linked list of holes
int current_hole = 0;

// Initialize memory block of size n/(d-v)
void initialize(int n, int d, int v, int** mem, int**blocks){
    srand(time(NULL));
    int max_blocks = n/(d-v);//why?!
    *mem = (int*)malloc(sizeof(int)*n);
    (*mem)[0] = (*mem)[n-1] = -n+2;
    (*mem)[1] = (*mem)[2] = 0;
    *blocks = (int*)malloc(max_blocks*sizeof(int));
}
int nrand_gen(int d, double v, int n){
    double rv;
    do{
        rv=  nrand() * v + d;//use rand function here
    }while(rv < 3 || rv > n-4);
    return rv;
}
int worstFit(int* blocks, int* block_count, int* mem, int d, double v, int n){
    if(head == -1)//no hole is found!
        return 0;
    int size = nrand_gen(d, v, n);  // Generate random size for block
    int iterator = head, pred = 0, succ = 0, new_size = 0, block_address = 0, bestHole = -1, bestSize  = 0;

    // Find the best fit for the new block
    while(1) {
        if (-mem[iterator] > bestSize)  // Hole length is larger than the best size
            bestSize = -mem[iterator];  // reassign bestSize to the largest hole size
        if (size < bestSize)            // Best size can still fit the new block size
            bestHole = iterator;        // Best hole is identified at the current iterator
        number_of_examined_holes++;
        iterator = mem[iterator + 2];
        if (iterator == head){
            iterator = bestHole;
            break;
        }
    }
    if (bestHole == -1)
        return 0; // No Hole

    // 4 is the min size for a block
    // Fill the hole completely
    if(abs(size+mem[iterator]) <= 4){   // mem[iterator] = size of available hole
        //request size is almost equal to the hole size
        size = -mem[iterator];  // size becomes the size of the hole
        mem[iterator] = mem[iterator + size + 1] = size;

        // Delete the current hole since it is full now
        pred = mem[iterator+1];
        succ = mem[iterator+2];
        if (pred == iterator)   // current hole is the only hole
            head = -1;
        else{
            mem[pred+2] = succ; // current->prev->next = current->next
            mem[succ+1] = pred; // current->next->prev = current->prev
        }
        block_address = iterator;
    }else{  //Fill partially
        mem[iterator] += (size+2);  // mem[iterator] = size of available hole
        //adding pos by neg to make it less negative
        new_size = -mem[iterator];
        mem[iterator + new_size + 1] = mem[iterator];
        //block starts at iterator + new_size + 2
        block_address = iterator + new_size + 2;
        mem[block_address] = mem[block_address + size + 1] = size;
    }
    blocks[*block_count] = block_address;
    (*block_count)++;
    return 1;//successful
}
void release(int* blocks, int* block_count, int* mem, int arrSize){
    if(!*block_count)   // No blocks to release
        return;
    int to_be_released = rand()%(*block_count); // Random choice of block to release
    printf("releasing block at address %d\n", blocks[to_be_released]);

    int blockSize = -mem[blocks[to_be_released]],
            leftLen = mem[blocks[to_be_released] - 1],
            rightLen = mem[(blocks[to_be_released] + -blockSize + 2) % arrSize],
            *pLeft = &leftLen, // block to the left
    *pRight = &rightLen,    // block to the right
    blockAddress = blocks[to_be_released];

    // Case 1: Both neighbors are blocks, create a new hole and add it to mem array.
    if (leftLen >= 0 && rightLen >= 0){
        //printf("Case 1 occurred.);
        while (*pLeft > 0)
            pLeft = &mem[*pLeft - *pLeft - 2];  // Traverse left through memory to find the closest hole to left
        pLeft = &mem[*pLeft - *pLeft - 1];      // pLeft points to 'next' pointer for leftmost hole
        mem[blockAddress + 1] = *pLeft;         // Join new hole with leftmost hole
        mem[*pLeft + 2] = blockSize;            // Join leftmost hole with new hole
        while (*pRight > 0)
            pRight = &mem[*pRight + *pRight + 2];   // Traverse left through memory to find the closest hole to left
        pRight = &mem[*pRight + 1];                 // pRight points to 'prev' pointer for rightmost hole
        mem[blockAddress + 2] = *pRight;            // Join new hole with rightmost hole
        *pRight = blockSize;                        // Join rightmost hole with new hole
    }
        // Case 2: Only left neighbor is a block. Merge the hole with the right hole.
    else if (leftLen >= 0 && rightLen < 0){
        //printf("Case 2 occurred.);
        mem[blockAddress] = rightLen;
        mem[blockAddress + 1] = mem[blockAddress + -mem[blockAddress] + 3];
        mem[blockAddress + 2] = mem[blockAddress + -mem[blockAddress] + 4];
    }
        // Case 3: Only right neighbor is a block. Merge the hole with the left hole.
    else if (leftLen < 0 && rightLen >= 0){
        //printf("Case 3 occurred.);
        mem[blockAddress + leftLen - 2] += -mem[blockAddress];  // Add new hole size to current hole
    }
        // Case 4: Both neighbors are holes, merge them together to form one large hole.
    else if (leftLen < 0 && rightLen < 0){
        //printf("Case 4 occurred.);
        mem[blockAddress + leftLen - 2] += -mem[blockAddress] + -rightLen;   // Update hole size
        mem[blockAddress + leftLen - 1] = mem[blockAddress + -mem[blockAddress] + 3];
        mem[blockAddress + leftLen] = mem[blockAddress + -mem[blockAddress] + 4];
    }

    //remove an integer at index to_be_released from blocks array...
    blocks[to_be_released] = blocks[(*block_count)-1];
    (*block_count)--;
}

// Main
int main(int argc, char** argv) {
    int x = 1000, n = 16777216, d = 1048576, original_x = x;
    double v = 264144.00;

    // CLA handling
    while(*++argv){
        //./main -x 1000 -n 16000000 -d 1000000 -v 256000
        if(**argv != '-')
            return 1;
        switch((*argv)[1]){
            case 'x':
                x = atoi(*++argv);
                break;
            case 'n':
                n = atoi(*++argv);
                break;
            case 'd':
                d = atoi(*++argv);
                break;
            case 'v':
                v = atof(*++argv);
                break;
            default: //error
                return 1;
        }
    }
    int* mem;   // the memory
    int* blocks;
    int block_count = 0;
    printf("Running the simulation with x=%d n=%d d=%d v=%.2f\n", x,n,d,v);
    initialize(n, d, v, &mem, &blocks);
    while(x > 0){
        while(worstFit(blocks, &block_count, mem, d, v, n));
        update_memory_utilization(blocks, block_count, mem, n, original_x);
        release(blocks,&block_count,mem, n);
        x--;
    }
    printf("Average utilization is %.3f\n", avg_utilization);
    printf("Average holes examined per request is %.3f", number_of_examined_holes/original_x);
    return 0;
}

void update_memory_utilization(int* blocks, int block_count, int* mem, int n, int x) {
    double utilization = 0;
    for (int i = 0; i < block_count; i++){
        utilization += mem[blocks[i]];
        //printf("mem[blocks[%d]] = %d at block address: %d\n",i, mem[blocks[i]], blocks[i]);
    }
    utilization /= n;
    //printf("%f\n",utilization);
    avg_utilization += utilization/x;
}
