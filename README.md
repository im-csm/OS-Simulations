# OS-Simulations
Through my Operating Systems course I have created two sets of simulations to learn more about how an OS works.

## CPU Scheduling Simulation
This simulation covered 4 algorithms that a CPU may use for scheduling processes competing for its processing power.
For each simulation, it can be assumed that there is only 1 core available and that only 1 process may be "executing" at a given time.

### FIFO
First In First out (FIFO) is a scheduling algorithm that gives all its processing power to the first process that shows up in the queue. 

### SJF
Shorted Job First (SJF) is a scheduling algorithm that seeks to complete the process with the shortest total time. This allows new proceeses to join the queue but will sort them based on their total time to completion, and will complete the processes with the shortest time regardless of when they have arrived in the queue.

### SRT
Shortest Remaining Time (SRT) is a scheduling algorithm that works similarly to SJF. The difference is that with this algorithm it will switch between processes in an attempt to clear out the queue as fast as possible.

For example:
If a process is currently running on the CPU with a remaining time of 5 seconds, and a new process enters the queue with a remaining time of 2 seconds, this new process will be given CPU time until it is completed and then the other process will resume.

### ML
Multi-level queue is a unique algorithm that works by having a queue of queues. All new processes will come in to the top level queue and be given a set amount of CPU time, for example 10 seconmds. Once it has run through its CPU time once, if it has remaining time to completion it will be placed on the next queue down.


## Memory Mangement Simulation
This simulation covered 4 memory management techniques that an OS may utilize to add new storage blocks to memory.
For each simulation it can be assumed that there is no function in place to shift memory blocks or close gaps and that the size of the memory is fixed.

### Best Fit
This algorithm seeks to find a hole in the memory that is the smallest hole available that can fit the new memory block being added. This alogorithm is a little bit slower, but can provide better memory utilization.

### Worst fit
This algorithm is the opposite of Best Fit and seeks to find the largest hole in memory to place the new block. This algorithm works fairly quickly but can lead to awkward holes in memory that may make it impossible for new memory blocks to be cleanly placed within memory. This would also require more effort when shifting exisiting blocks around to condense the memory later on.

### First Fit
This algorithm is designed to find the first open hole in memory to fit the new block, regardless of the size or number of other holes in the memory. It can work quickly but can quickly slow down when a block of memory cannot be added to the beginning, leading to either traversal of the entire memory block or requiring the memory blocks to be condensed.

### Next Fit
This algorithm works like First Fit in the sense that it first finds a hole in memory where the new block can be placed, but once a new block of memory is to be added it begins its search from where the last block was added. This can be efficient initially, however if a new block of memory is too large for any of the remaining holes in the memory it will have to traverse the entirety of the memory to get back to the beginning where there may be a hole large enough to fit it.
