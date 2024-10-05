# CS3113-project-1
Shared memory Process synchronization

Overview
This program demonstrates the use of shared memory for process synchronization using fork() to create child processes. Each child increments a shared variable in parallel, and the parent waits for each child process to complete before printing their Process IDs (PIDs).

The shared memory allows multiple child processes to work on the same variable simultaneously. The program uses several system calls, such as shmget(), shmat(), shmdt(), and shmctl(), for handling shared memory and process synchronization using fork().

Files
main.c: The main source code file which contains the implementation of shared memory and child process creation.
Compilation Instructions
To compile and execute the program, follow these steps:

1. Install Required Libraries
Ensure that you have the necessary libraries installed (specifically glibc and POSIX libraries, which are typically included with a standard Linux distribution). The code uses system calls from <sys/shm.h>, <sys/ipc.h>, and <sys/wait.h>, which are part of the standard library for IPC (Inter-Process Communication).

2. Compile the Program
Open a terminal and navigate to the directory containing main.c. Use the following command to compile the code using gcc:

gcc -o shared_memory_program main.c

This command will create an executable named shared_memory_program. If there are any compilation errors, the terminal will display them, and you can make necessary corrections.

3. Execute the Program
After compiling, run the program using the following command:
./shared_memory_program

When you execute the program, it will create four child processes, each incrementing a shared variable by different amounts:

Process 1 increments the shared variable 100,000 times.
Process 2 increments the shared variable 200,000 times.
Process 3 increments the shared variable 300,000 times.
Process 4 increments the shared variable 500,000 times.
The parent process waits for each child to complete and prints a message with the child’s PID upon completion.

4. Program Output
The program will output the PIDs of the child processes as they complete. The final message will indicate the end of the program. Sample output:

Child with pid 12345 has just exited.
Child with pid 12346 has just exited.
Child with pid 12347 has just exited.
Child with pid 12348 has just exited.
End of Program

5. Clean Up
   
The program automatically detaches the shared memory segment and deletes it after use. However, if you encounter any errors and the program terminates unexpectedly, you can manually clean up shared memory using the following commands:

List all shared memory segments:
ipcs -m

Remove a specific shared memory segment:
ipcrm -m <shmid>
Where <shmid> is the shared memory segment ID shown in the output of the ipcs -m command.

Additional Information
Shared Memory (shmget, shmat, shmdt, shmctl): The program uses shmget() to create a shared memory segment and shmat() to attach it to the process's address space. After the child processes finish, the shared memory is detached using shmdt(), and it is removed using shmctl().

Fork and Wait: The fork() system call creates child processes, and waitpid() is used by the parent to wait for each child process to terminate before moving on to the next.

Error Handling
The program includes error handling for all major system calls such as shmget(), shmat(), fork(), and waitpid(). If an error occurs, an appropriate message will be printed using perror() and the program will terminate with an exit code.
