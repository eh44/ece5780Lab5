#include <stdio.h>
#include <stdlib.h>
#include "task.h" //look here to see how the taskset of structs is defined

int read_input_file(const char *filename, TaskSet *taskset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open input file.\n");
        return -1;
    }

    // Read number of periodic tasks
    if (fscanf(file, "%d", &taskset->num_periodic) != 1) {
        fprintf(stderr, "Error: Invalid format (num_periodic).\n");
        fclose(file);
        return -1;
    }

    // Read simulation time
    if (fscanf(file, "%d", &taskset->sim_time) != 1) {
        fprintf(stderr, "Error: Invalid format (simulation time).\n");
        fclose(file);
        return -1;
    }

    // Read periodic tasks
    for (int i = 0; i < taskset->num_periodic; i++) {
        char id;
        int exec, period;

        if (fscanf(file, " %c , %d , %d", &id, &exec, &period) != 3) {
            fprintf(stderr, "Error: Invalid format in periodic task %d.\n", i);
            fclose(file);
            return -1;
        }

        taskset->periodic[i].id = id;
        taskset->periodic[i].execution_time = exec;
        taskset->periodic[i].period = period;
        taskset->periodic[i].absolute_deadline = period;
    }

    // Read number of aperiodic tasks
    if (fscanf(file, "%d", &taskset->num_aperiodic) != 1) {
        fprintf(stderr, "Error: Invalid format (num_aperiodic).\n");
        fclose(file);
        return -1;
    }

    // Read aperiodic tasks
    for (int i = 0; i < taskset->num_aperiodic; i++) {
        char id;
        int exec, release;

        if (fscanf(file, " %c , %d , %d", &id, &exec, &release) != 3) {
            fprintf(stderr, "Error: Invalid format in aperiodic task %d.\n", i);
            fclose(file);
            return -1;
        }

        taskset->aperiodic[i].id = id;
        taskset->aperiodic[i].execution_time = exec;
        taskset->aperiodic[i].release_time = release;
        taskset->aperiodic[i].absolute_deadline = release + 500; // implicit deadline of 500
    }

    fclose(file);
    return 0;
}