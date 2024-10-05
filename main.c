// Thomas Duffy username: cs027
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#define SHMKEY ((key_t) 1497)

// Define shared memory structure to store final values
typedef struct {
    int total;
    int counters[4];  // To store the final counter values for each process
} shared_memory;

void process1(shared_memory *total) {
    while (total->total < 100000) {
        total->total++;  // Increment the shared memory value
    }
    total->counters[0] = total->total;  // Store final value for process 1
}

void process2(shared_memory *total) {
    while (total->total < 200000) {
        total->total++;  // Increment the shared memory value
    }
    total->counters[1] = total->total;  // Store final value for process 2
}

void process3(shared_memory *total) {
    while (total->total < 300000) {
        total->total++;  // Increment the shared memory value
    }
    total->counters[2] = total->total;  // Store final value for process 3
}

void process4(shared_memory *total) {
    while (total->total < 500000) {
        total->total++;  // Increment the shared memory value
    }
    total->counters[3] = total->total;  // Store final value for process 4
}

int main() {
    int shmid, pid1, pid2, pid3, pid4, status;
    shared_memory *total;
    char *shmadd = (char *) 0;  // Initialize the shared memory address

    // Create and connect to a shared memory segment
    if ((shmid = shmget(SHMKEY, sizeof(shared_memory), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory to the address space of the process
    if ((total = (shared_memory *) shmat(shmid, shmadd, 0)) == (shared_memory *) -1) {
        perror("shmat");
        exit(0);
    }

    // Initialize shared variable "total" and process counters to 0
    total->total = 0;
    for (int i = 0; i < 4; i++) {
        total->counters[i] = 0;
    }

    // Create the first child process (process1)
    if ((pid1 = fork()) == 0) {
        process1(total);
        exit(0);  // Child exits after completing its task
    }
    // Parent waits for the first child process to finish
    waitpid(pid1, &status, 0);

    // Create the second child process (process2)
    if ((pid2 = fork()) == 0) {
        process2(total);
        exit(0);  // Child exits after completing its task
    }
    // Parent waits for the second child process to finish
    waitpid(pid2, &status, 0);

    // Create the third child process (process3)
    if ((pid3 = fork()) == 0) {
        process3(total);
        exit(0);  // Child exits after completing its task
    }
    // Parent waits for the third child process to finish
    waitpid(pid3, &status, 0);

    // Create the fourth child process (process4)
    if ((pid4 = fork()) == 0) {
        process4(total);
        exit(0);  // Child exits after completing its task
    }
    // Parent waits for the fourth child process to finish
    waitpid(pid4, &status, 0);

    // After all child processes have finished, print the counters in the correct order
    printf("From Process 1: counter = %d.\n", total->counters[0]);
    printf("From Process 2: counter = %d.\n", total->counters[1]);
    printf("From Process 3: counter = %d.\n", total->counters[2]);
    printf("From Process 4: counter = %d.\n", total->counters[3]);

    // Now print the child exit messages in the order they exited
    printf("Child with ID: %d has just exited.\n", pid1);
    printf("Child with ID: %d has just exited.\n", pid2);
    printf("Child with ID: %d has just exited.\n", pid3);
    printf("Child with ID: %d has just exited.\n", pid4);

    // Detach the shared memory from the parent process
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(-1);
    }

    printf("End of Program.\n");
    return 0;
}
