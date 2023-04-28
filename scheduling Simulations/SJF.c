#include <math.h>
#include <time.h>
#include "pa1.h"
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))

heap generate_arrival_times(int n, int k){
    heap h = create_heap(n);
    for(int i = 0; i < n;i++)
        min_insert(&h, frand()*k);
    return h;
}

// int argc, char** argv
int main() {
    srand(time(NULL));
    int n, k;
    printf("Enter n,k:");
    scanf("%d,%d", &n, &k);
    /*
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
    */
    FILE *fp = fopen("SJF_ATT.txt", "a");
    fprintf(fp, "(n, k) = (%d, %d):\n", n, k);
    for (int o = 1; o <= 25; o++) {
        int d = (o * k) / n;
        double v = d / 4.0;
        heap arrival_times = generate_arrival_times(n, k);
        printf("# of processes: %d\nFirst 20 arrive times:\n", arrival_times.size);
        heap copy = clone(arrival_times);
        for (int i = 0; i < 20; i++)
            printf("%d ", min_delete(&copy));
        putchar('\n');

        // Shortest Job first using a min heap of process burst times
        pheap burst_times = create_pheap(n);
        int t = 0;  //time of my simulator
        double att = 0.0;   //keeps track of TTs sum
        int process_count = 0;
        process *current = NULL;
        while (!current || t < k || !pheap_is_empty(burst_times)) {
            while (t == get_min(arrival_times)) {     //new process arrives
                process p;
                p.arrival_time = t;
                p.remaining_time = p.burst_time = (int) round(nrand() * v + d);
                p.tt = 0;
                p.priority_level = rand() % 10 + 1;
                pheap_min_insert(&burst_times, p);
                min_delete(&arrival_times);
                printf("t = %d: A new process admitted, bt = %d\n", t, p.burst_time);
                printf("Current pheap min time = %d\n", pheap_get_min(burst_times).burst_time);
            }
            if (current == NULL && !pheap_is_empty(burst_times)) {  // Current points to
                current = (process *) malloc(sizeof(process));
                *current = pheap_min_delete(&burst_times);
            }
            if (current != NULL) {
                current->remaining_time--;
                if (current->remaining_time == 0) {
                    current->tt = (t + 1) - current->arrival_time; //termination-arrival
                    printf("P# = %d, t = %d: A process with arrival time=%d and bt=%d got terminated with TT=%d\n",
                           ++process_count, t + 1, current->arrival_time, current->burst_time, current->tt);
                    //fprintf(fp, "Arrival:%d, bt:%d, TT:%d\n", current->arrival_time, current->burst_time, current->tt);
                    att += current->tt;
                    free(current);
                    current = NULL;
                }
            }
            t++;
        }
        printf("FIFO Algorithm for (n,k)=(%d,%d): ATT= %.3f, d= %d, d/ATT= %.3f\n", n, k, att / n, d, d * n / att);
        fprintf(fp, "%.3f\n", d * n / att);
        free(current);
        current = NULL;
        t = v = d = att = 0;
    }
    fprintf(fp, "\n");
    fclose(fp);
    return 0;
}

