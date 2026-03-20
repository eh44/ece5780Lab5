#ifndef TASK_H
#define TASK_H

#define MAX_TASKS 100

typedef struct {
    char id;
    int execution_time;
    int period;
    int relative_deadline;
    int priority;
    int remaining_time;   
    int absolute_deadline; 
    int is_active;
} PeriodicTask;

typedef struct {
    char id;
    int execution_time;
    int release_time;
    int deadline;
    int priority;
    int remaining_time;  
    int absolute_deadline;
    int is_active;
} AperiodicTask;

typedef struct {
    int num_periodic;
    int num_aperiodic;
    int sim_time;

    PeriodicTask periodic[MAX_TASKS];
    AperiodicTask aperiodic[MAX_TASKS];
} TaskSet;

#endif