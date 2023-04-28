#include <math.h>
#include <time.h>
#include "pa1.h"
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))

heap generate_arrival_times(int n, int k)   // Generate heap of arrival times
{
    heap h = create_heap(n);
    for (int i = 0; i < n; i++)
        min_insert(&h, frand() * k);
    return h;
}

int highestPriority(lnklst_queue* array)    // Returns highest priority level with a process in it.
{
    for (int i = 0; i <= 10; i++) {
        if (!is_empty2(array[i]))
            return i;
    }
}

// int argc, char** argv
// n -> {100, 500, 500, 1000}
// k -> {1000, 10000, 5000, 10000}
int main() {
    srand(time(NULL));
    int n = 1000;
    int k = 10000;
    /*
    int n, k;
    while(*++argv){     // Get command line arguments
        if(**argv == '-' && (*argv)[1] == 'n')
            n = atoi(*++argv);
        else if(**argv == '-' && (*argv)[1] == 'k')
            k = atoi(*++argv);
        else if(**argv == '-' && (*argv)[1] == 'd')
            d = atoi(*++argv);
        else
            continue;
    }
    int d = k / n;
    */

    FILE *fp = fopen("ML_ATT.txt", "a");
    fprintf(fp, "(n, k) = (%d, %d):\n", n, k);
    for (int o = 1; o <= 25; o++){
        int d = (o * k) / n;
        double v = d / 4.0;
        heap arrival_times = generate_arrival_times(n, k);
        /*
        printf("# of processes: %d\nFirst %d arrive times:\n", arrival_times.size, n);
        heap copy = clone(arrival_times);
        for (int i = 0; i < n; i++)
            printf("%d ", min_delete(&copy));
        putchar('\n');
        */

        // ML using linked list array or linked list queues for priority levels
        lnklst_queue *lnklst_array = (lnklst_queue *) malloc(10 * sizeof(lnklst_queue));
        for (int l = 0; l <= 10; l++)
            lnklst_array[l] = create_queue2(n);
        int t = 0;  //time of my simulator
        double att = 0.0;   //keeps track of TTs sum
        int process_count = 0;
        int qCounter = 0;
        process *current = NULL;
        while (!current || t < k || !is_empty2(lnklst_array[highestPriority(lnklst_array)])) {
            while (t == get_min(arrival_times)) {     //new process arrives
                process p;
                p.arrival_time = t;
                p.remaining_time = p.burst_time = (int) round(nrand() * v + d);
                p.tt = 0;
                p.priority_level = rand() % 10 + 1;
                //printf("New process with priority level: %d\n", p.priority_level);
                enqueue2(&lnklst_array[p.priority_level], p);
                //printf("t = %d: A new process admitted to n%d, bt = %d.\n", t, p.priority_level, p.burst_time);
                min_delete(&arrival_times);
            }
            if (current == NULL && !is_empty2(lnklst_array[highestPriority(lnklst_array)])) {
                current = (process *) malloc(sizeof(process));
                *current = dequeue2(&lnklst_array[highestPriority(lnklst_array)]);
            }
            if (current != NULL) {
                if (current->priority_level > highestPriority(lnklst_array)) {
                    enqueue2(&lnklst_array[current->priority_level], *current);
                    *current = dequeue2(&lnklst_array[highestPriority(lnklst_array)]);
                }
                current->remaining_time--;
                if (current->remaining_time == 0) {
                    current->tt = (t + 1) - current->arrival_time; //termination-arrival
                    //printf("P# = %d, t = %d: A process with arrival time=%d, bt=%d, priority = %d got terminated with TT=%d\n",
                           //++process_count, t + 1, current->arrival_time, current->burst_time, current->priority_level,
                           //current->tt);
                    //fprintf(fp, "Arrival:%d, bt:%d, TT:%d\n", current->arrival_time, current->burst_time, current->tt);
                    att += current->tt;
                    free(current);
                    current = NULL;
                }
            }
            t++;
        }
        printf("ML Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.3f\n", n, k, att/n, d, d*n/att);
        fprintf(fp, "%.3f\n", d*n/att);
        free(lnklst_array);
        free(current);
        lnklst_array = NULL;
        current = NULL;
        t = v = d = att = 0;
    }
    fclose(fp);
    return 0;
}

