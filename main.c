#include <stdio.h>
#include "file_extract.h"

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

    // Now taskset is filled and ready
}