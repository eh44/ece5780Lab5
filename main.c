#include <stdio.h>
#include <stdlib.h>
#include "file_extract.h"
#include "arraylist.h"

struct Node {
    PeriodicTask task;
    AperiodicTask atask;
    struct Node *next;
};

struct ReadyPool {
    int* readyPool;
    int size;
};

int main(int argc, char *argv[]) {
    TaskSet taskset;

    // Expect 3 arguments "./scheduler.exe input.txt output.txt"
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.txt output.txt\n", argv[0]);
        return 1;
    }

    // Read input file
    if (read_input_file(argv[1], &taskset) != 0) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }

    FILE *outFile = fopen(argv[2], "w");
    if (outFile == NULL) {
        fprintf(stderr, "Error opening output file\n");
        return 1;
    }

    for(int i = 0; i < 3; i++) {
        int totalMisses = 0;
        int totalPreemptions = 0;
        
        int lastRunType = -1; 
        int lastRunIdx = -1;

        ArrayList *readyQueuePeriodic = create_arraylist(TYPE_INT);
        ArrayList *readyQueueAperiodic = create_arraylist(TYPE_INT);

        if(i == 0) {
            fprintf(outFile, "\n=== Rate Monotonic (RM) Schedule ===\n");
            for(int j = 0; j < taskset.num_periodic; j++) {
                for(int k = j; k < taskset.num_periodic; k++) {
                    if(taskset.periodic[k].period < taskset.periodic[j].period) {
                        PeriodicTask temp = taskset.periodic[j];
                        taskset.periodic[j] = taskset.periodic[k];
                        taskset.periodic[k] = temp;
                    }
                }
            }
            for(int j = 0; j < taskset.num_periodic; j++) {
                taskset.periodic[j].priority = j;
            }
            for(int j = 0; j < taskset.num_aperiodic; j++) {
                taskset.aperiodic[j].priority = j + taskset.num_periodic;
            }
        } else if(i == 1) {
            fprintf(outFile, "\n=== Earliest Deadline First (EDF) Schedule ===\n");
        } else {
            fprintf(outFile, "\n=== Least Laxity First (LLF) Schedule ===\n");
        }

        for(int j = 0; j < taskset.sim_time; j++) {
            
            for(int k = 0; k < taskset.num_periodic; k++) {
                if(j >= taskset.periodic[k].absolute_deadline && taskset.periodic[k].remaining_time > 0) {
                    totalMisses++;
                    fprintf(outFile, "Time %d: MISS - Periodic Task %c missed its deadline!\n", j, taskset.periodic[k].id);
                }
            }
            for(int k = 0; k < taskset.num_aperiodic; k++) {
                if(j >= taskset.aperiodic[k].absolute_deadline && taskset.aperiodic[k].remaining_time > 0) {
                    totalMisses++;
                    fprintf(outFile, "Time %d: MISS - Aperiodic Task %c missed its deadline!\n", j, taskset.aperiodic[k].id);
                }
            }

            for(int k = 0; k < taskset.num_periodic; k++) {
                if (j % taskset.periodic[k].period == 0) {
                    taskset.periodic[k].remaining_time = taskset.periodic[k].execution_time;
                    taskset.periodic[k].absolute_deadline = j + taskset.periodic[k].period;
                    taskset.periodic[k].is_active = 1;
                    int index = k;
                    arraylist_add(readyQueuePeriodic, &index);
                }
            }

            for(int k = 0; k < taskset.num_aperiodic; k++) {
                if (j == taskset.aperiodic[k].release_time) {
                    taskset.aperiodic[k].remaining_time = taskset.aperiodic[k].execution_time;
                    taskset.aperiodic[k].absolute_deadline = j + 500;
                    int index = k;
                    arraylist_add(readyQueueAperiodic, &index);
                }
            }

            if(readyQueuePeriodic->length == 0 && readyQueueAperiodic->length == 0) {
                lastRunType = -1;
                lastRunIdx = -1;
                continue; 
            }

            int currentTask = 0;
            if (readyQueuePeriodic->length > 0) {
                for(int k = 0; k < readyQueuePeriodic->length; k++) {
                    int currIdx = *(int*)arraylist_get(readyQueuePeriodic, currentTask);
                    int evalIdx = *(int*)arraylist_get(readyQueuePeriodic, k);

                    if(i == 0 && taskset.periodic[currIdx].priority > taskset.periodic[evalIdx].priority) { currentTask = k; }
                    else if(i == 1 && taskset.periodic[evalIdx].absolute_deadline < taskset.periodic[currIdx].absolute_deadline) { currentTask = k; }
                    else if(i == 2) {
                        int laxCurr = taskset.periodic[currIdx].absolute_deadline - taskset.periodic[currIdx].remaining_time - j;
                        int laxEval = taskset.periodic[evalIdx].absolute_deadline - taskset.periodic[evalIdx].remaining_time - j;
                        if(laxEval < laxCurr) { currentTask = k; }
                    }
                }
            }

            int currentATask = 0;
            if (readyQueueAperiodic->length > 0) {
                for(int k = 0; k < readyQueueAperiodic->length; k++) {
                    int currIdx = *(int*)arraylist_get(readyQueueAperiodic, currentATask);
                    int evalIdx = *(int*)arraylist_get(readyQueueAperiodic, k);

                    if(i == 0 && taskset.aperiodic[currIdx].priority > taskset.aperiodic[evalIdx].priority) { currentATask = k; }
                    else if(i == 1 && taskset.aperiodic[evalIdx].absolute_deadline < taskset.aperiodic[currIdx].absolute_deadline) { currentATask = k; }
                    else if(i == 2) {
                        int laxCurr = taskset.aperiodic[currIdx].absolute_deadline - taskset.aperiodic[currIdx].remaining_time - j;
                        int laxEval = taskset.aperiodic[evalIdx].absolute_deadline - taskset.aperiodic[evalIdx].remaining_time - j;
                        if(laxEval < laxCurr) { currentATask = k; }
                    }
                }
            }

            int runPeriodic = 1; 
            if (readyQueuePeriodic->length > 0 && readyQueueAperiodic->length == 0) { runPeriodic = 1; } 
            else if (readyQueueAperiodic->length > 0 && readyQueuePeriodic->length == 0) { runPeriodic = 0; } 
            else {
                int pIdx = *(int*)arraylist_get(readyQueuePeriodic, currentTask);
                int aIdx = *(int*)arraylist_get(readyQueueAperiodic, currentATask);
                
                if (i == 0 && taskset.periodic[pIdx].priority > taskset.aperiodic[aIdx].priority) { runPeriodic = 0; } 
                else if (i == 1 && taskset.periodic[pIdx].absolute_deadline > taskset.aperiodic[aIdx].absolute_deadline) { runPeriodic = 0; } 
                else if (i == 2) {
                    int laxityP = taskset.periodic[pIdx].absolute_deadline - taskset.periodic[pIdx].remaining_time - j;
                    int laxityA = taskset.aperiodic[aIdx].absolute_deadline - taskset.aperiodic[aIdx].remaining_time - j;
                    if (laxityP > laxityA) { runPeriodic = 0; }
                }
            }

            if (runPeriodic == 1) {
                int pIdx = *(int*)arraylist_get(readyQueuePeriodic, currentTask);
                
                if (lastRunType != -1 && (lastRunType != 0 || lastRunIdx != pIdx)) {
                    if (lastRunType == 0 && taskset.periodic[lastRunIdx].remaining_time > 0) {
                        totalPreemptions++;
                        fprintf(outFile, "Time %d: PREEMPT - Periodic Task %c preempted by Periodic Task %c\n", j, taskset.periodic[lastRunIdx].id, taskset.periodic[pIdx].id);
                    } else if (lastRunType == 1 && taskset.aperiodic[lastRunIdx].remaining_time > 0) {
                        totalPreemptions++;
                        fprintf(outFile, "Time %d: PREEMPT - Aperiodic Task %c preempted by Periodic Task %c\n", j, taskset.aperiodic[lastRunIdx].id, taskset.periodic[pIdx].id);
                    }
                }

                fprintf(outFile, "Time %d: RUN - Periodic Task %c\n", j, taskset.periodic[pIdx].id);
                taskset.periodic[pIdx].remaining_time--;
                
                if (taskset.periodic[pIdx].remaining_time == 0) {
                    arraylist_remove(readyQueuePeriodic, currentTask);
                }
                
                lastRunType = 0;
                lastRunIdx = pIdx;

            } else {
                int aIdx = *(int*)arraylist_get(readyQueueAperiodic, currentATask);
                
                if (lastRunType != -1 && (lastRunType != 1 || lastRunIdx != aIdx)) {
                    if (lastRunType == 0 && taskset.periodic[lastRunIdx].remaining_time > 0) {
                        totalPreemptions++;
                        fprintf(outFile, "Time %d: PREEMPT - Periodic Task %c preempted by Aperiodic Task %c\n", j, taskset.periodic[lastRunIdx].id, taskset.aperiodic[aIdx].id);
                    } else if (lastRunType == 1 && taskset.aperiodic[lastRunIdx].remaining_time > 0) {
                        totalPreemptions++;
                        fprintf(outFile, "Time %d: PREEMPT - Aperiodic Task %c preempted by Aperiodic Task %c\n", j, taskset.aperiodic[lastRunIdx].id, taskset.aperiodic[aIdx].id);
                    }
                }

                fprintf(outFile, "Time %d: RUN - Aperiodic Task %c\n", j, taskset.aperiodic[aIdx].id);
                taskset.aperiodic[aIdx].remaining_time--;
                
                if (taskset.aperiodic[aIdx].remaining_time == 0) {
                    arraylist_remove(readyQueueAperiodic, currentATask);
                }

                lastRunType = 1;
                lastRunIdx = aIdx;
            }
        } 
        
        fprintf(outFile, "\n--- Summary ---\n");
        fprintf(outFile, "Total Preemptions: %d\n", totalPreemptions);
        fprintf(outFile, "Total Deadline Misses: %d\n\n", totalMisses);
    } 

    fclose(outFile);
    return 0;
}