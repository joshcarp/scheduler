
#include "scheduler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* print_stats prints statistics on the head node*/
void print_stats (process *head, int time)
{
    int intervals = ceiling (((float)time / 60));
    int *throughput = (int *)calloc (intervals, sizeof (int) * (intervals));
    int total_turnaroundtime = 1;
    float time_overhead_max = 0;
    float time_overhead_total = 0;
    float overhead;
    int num = 0;
    process *next = head;
    while (next)
    {
        throughput[((next->last_execution_time - 1) / 60)] += 1;
        total_turnaroundtime += next->last_execution_time - next->arrival;
        overhead = (float)(next->last_execution_time - next->arrival) / next->jobtime;
        if (overhead > time_overhead_max)
        {
            time_overhead_max = overhead;
        }
        time_overhead_total += overhead;
        num++;
        next = next->llNext;
    }
    int max_throughput = -1;
    int min_throughput = -1;
    float ave_throughput = (float)num / intervals;
    for (int i = 0; i < intervals; i++)
    {
        if (throughput[i] > max_throughput || max_throughput == -1)
        {
            max_throughput = throughput[i];
        }
        if (throughput[i] < min_throughput || min_throughput == -1)
        {
            min_throughput = throughput[i];
        }
    }
    printf ("Throughput %.f, %d, %d\n", round_5 (ave_throughput), min_throughput, max_throughput);
    printf ("Turnaround time %.f\n", round_5 ((float)total_turnaroundtime / (float)num));
    printf ("Time overhead %.2f %.2f\n", time_overhead_max, time_overhead_total / num);
    printf ("Makespan %d\n", time);
    free (throughput);
    freeData (head);
}

/* print_addresses prints all of the elements in arr that are equal to allocated */
void print_addresses (page **arr, uint n, bool allocated)
{
    printf ("mem-addresses=[");
    page *temp;
    for (uint i = 0; i < n; ++i)
    {
        for (uint j = i + 1; j < n; ++j)
        {
            if (arr[i] != NULL && arr[j] != NULL)
            {
                if (arr[i]->id > arr[j]->id)
                {
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
        }
    }
    char comma[3] = "";
    for (uint i = 0; i < n; i++)
    {
        if (arr[i]->allocated != allocated)
        {
            continue;
        }
        printf ("%s%u", comma, arr[i]->id);
        if (i != n - 1)
        {
            strcpy (comma, ",");
        }
    }
    printf ("]");
}

/* print_evicted prints the recently evicted processes in memory */
void print_evicted (mem *memory, int time)
{
    if (memory->num_recently_evicted > 0)
    {
        printf ("%d, EVICTED, ", time);
        print_addresses (memory->recently_evicted, memory->num_recently_evicted, false);
        printf ("\n");
    }
}
