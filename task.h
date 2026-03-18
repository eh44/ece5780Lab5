#ifndef TASK_H
#define TASK_H

#define MAX_TASKS 100

typedef struct {
    char id;
    int execution_time;
    int period;
} PeriodicTask;

typedef struct {
    char id;
    int execution_time;
    int release_time;
    int deadline;
} AperiodicTask;

typedef struct {
    int num_periodic;
    int num_aperiodic;
    int sim_time;

    PeriodicTask periodic[MAX_TASKS];
    AperiodicTask aperiodic[MAX_TASKS];
} TaskSet;

#endif