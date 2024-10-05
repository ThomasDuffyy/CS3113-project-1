// Thomas Duffy username: cs027
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#define SHMKEY ((key_t) 1497)

// Define shared memory structure
typedef struct {
    int value;
} shared_memory;

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

    // Initialize shared variable "total" to 0
    total->value = 0;

    // Create the first child process (process1) to increment the counter to 100000
    if ((pid1 = fork()) == 0) {
        while (total->value < 100000) {
            total->value++;  // Increment the shared memory value
        }
        printf("From Process 1: counter = %d.\n", total->value);
        exit(0);  // Child exits after completing its task
    }

    // Create the second child process (process2) to increment the counter to 200000
    if ((pid2 = fork()) == 0) {
        while (total->value < 200000) {
            total->value++;  // Increment the shared memory value
        }
        printf("From Process 2: counter = %d.\n", total->value);
        exit(0);  // Child exits after completing its task
    }

    // Create the third child process (process3) to increment the counter to 300000
    if ((pid3 = fork()) == 0) {
        while (total->value < 300000) {
            total->value++;  // Increment the shared memory value
        }
        printf("From Process 3: counter = %d.\n", total->value);
        exit(0);  // Child exits after completing its task
    }

    // Create the fourth child process (process4) to increment the counter to 500000
    if ((pid4 = fork()) == 0) {
        while (total->value < 500000) {
            total->value++;  // Increment the shared memory value
        }
        printf("From Process 4: counter = %d.\n", total->value);
        exit(0);  // Child exits after completing its task
    }

    // Parent waits for all child processes to finish and prints their PIDs
    waitpid(pid1, &status, 0);
    printf("Child with pid %d has just exited.\n", pid1);

    waitpid(pid2, &status, 0);
    printf("Child with pid %d has just exited.\n", pid2);

    waitpid(pid3, &status, 0);
    printf("Child with pid %d has just exited.\n", pid3);

    waitpid(pid4, &status, 0);
    printf("Child with pid %d has just exited.\n", pid4);

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

