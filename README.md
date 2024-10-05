# CS3113-project-1
Cooperative Processes with Shared Memory

Overview
This project demonstrates the creation of four cooperating processes that increment a shared variable (total) using shared memory and semaphores for synchronization. Each child process increments the variable by 100,000, 200,000, 300,000, and 500,000, respectively. After completing, the parent waits for each child to exit, prints their final counter values, and manages the release of shared memory and resources.

The parent process ensures all child processes finish, prints their results, and cleans up the shared memory and semaphores after completion. The program uses several system calls, such as shmget(), shmat(), shmdt(), and shmctl(), for handling shared memory and process synchronization using fork().

Directory Structure
The project is located in a directory called cs3113-project and contains the following file:
project1.c — Source code of the project.

Compilation and Execution Instructions
Step 1: Navigate to the Project Directory
First, open a terminal and navigate to the directory where the project is located:

cd ~/cs3113-project
Make sure that the project1.c file is in this directory by listing the files:
ls
You should see project1.c in the output.

Step 2: Compile the Source Code
To compile the code, use the GNU C Compiler (gcc). Run the following command to compile project1.c:

gcc -o cs3113-project project1.c
If there are no errors, this command will create an executable file named cs3113-project.

Once the program is successfully compiled, run it by executing the following command:

./cs3113-project

This will start the program, creating 4 child processes, which increment the shared total variable, print the results, and then print when each child process exits.

Additional notes
Shared Memory & Semaphores: The code uses shared memory for the total variable, and semaphores are used to synchronize access to the shared memory across multiple processes.

Child Processes: Each child process increments the shared memory value by a predefined amount (100,000, 200,000, 300,000, and 500,000). The parent waits for all children to finish using the waitpid() function.

Memory Management: The parent ensures that shared memory is detached and removed, and the semaphore is cleared once all processes are finished.


