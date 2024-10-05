// Thomas Duffy username: cs027
// This program creates 4 child processes. Each child increments a shared variable 'total' by 100,000, 
// 200,000, 300,000, and 500,000 respectively. After each child finishes, the parent prints the process ID 
// of each child and the final values of the shared variable. The parent also releases the shared memory 
// and semaphores before ending the program.

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SHMKEY ((key_t) 1497)
#define SEMKEY ((key_t) 1234)

// Define shared memory structure to store final values
typedef struct {
    int total;
    int counters[4];  // To store the final counter values for each process
} shared_memory;

// Semaphore helper functions to synchronize access to shared memory
void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};  // Decrease semaphore value
    semop(semid, &sb, 1);
}

void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};   // Increase semaphore value
    semop(semid, &sb, 1);
}

// Each process function increments the shared variable 'total'
void process1(shared_memory *total, int semid) {
    while (total->total < 100000) {
        sem_wait(semid);  // Wait for access to shared memory
        total->total++;   // Increment the shared memory value
        sem_signal(semid); // Release access to shared memory
    }
    total->counters[0] = total->total;  // Store final value for process 1
}

// Similar to process1, but increments total to 200,000
void process2(shared_memory *total, int semid) {
    while (total->total < 200000) {
        sem_wait(semid);
        total->total++;
        sem_signal(semid);
    }
    total->counters[1] = total->total;
}

// Similar to process1, but increments total to 300,000
void process3(shared_memory *total, int semid) {
    while (total->total < 300000) {
        sem_wait(semid);
        total->total++;
        sem_signal(semid);
    }
    total->counters[2] = total->total;
}

// Similar to process1, but increments total to 500,000
void process4(shared_memory *total, int semid) {
    while (total->total < 500000) {
        sem_wait(semid);
        total->total++;
        sem_signal(semid);
    }
    total->counters[3] = total->total;
}

int main() {
    int shmid, semid, pid1, pid2, pid3, pid4, status;
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

    // Create semaphore
    if ((semid = semget(SEMKEY, 1, IPC_CREAT | 0666)) == -1) {
        perror("semget");
        exit(1);
    }
    semctl(semid, 0, SETVAL, 1);  // Initialize semaphore to 1

    // Fork 4 child processes
    if ((pid1 = fork()) == 0) {
        process1(total, semid);  // Child 1 calls process1
        exit(0);
    }

    if ((pid2 = fork()) == 0) {
        process2(total, semid);  // Child 2 calls process2
        exit(0);
    }

    if ((pid3 = fork()) == 0) {
        process3(total, semid);  // Child 3 calls process3
        exit(0);
    }

    if ((pid4 = fork()) == 0) {
        process4(total, semid);  // Child 4 calls process4
        exit(0);
    }

    // Parent waits for all children to finish
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);
    waitpid(pid4, &status, 0);

    // Print the counters in the correct order
    printf("From Process 1: counter = %d.\n", total->counters[0]);
    printf("From Process 2: counter = %d.\n", total->counters[1]);
    printf("From Process 3: counter = %d.\n", total->counters[2]);
    printf("From Process 4: counter = %d.\n", total->counters[3]);

    // Print child exit messages in the order they finished
    printf("Child with ID: %d has just exited.\n", pid1);
    printf("Child with ID: %d has just exited.\n", pid2);
    printf("Child with ID: %d has just exited.\n", pid3);
    printf("Child with ID: %d has just exited.\n", pid4);

    // Detach and remove shared memory
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(-1);
    }

    // Remove the semaphore
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-1);
    }

    printf("End of Program.\n");
    return 0;
}
