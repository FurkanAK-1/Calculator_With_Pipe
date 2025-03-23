#include <stdio.h>  // Standard input/output functions
#include <stdlib.h> // General utility functions
#include <unistd.h> // POSIX API for fork, exec, etc.
#include <string.h> // String manipulation functions
#include <sys/types.h> // Data types for system calls
#include <sys/wait.h>  // Wait for process termination
#include <sys/stat.h> // File system and FIFO handling
#include <fcntl.h> // File control options

int main() {
    char *fifos[] = {
        "calc_fifo_addition", // FIFO for addition 
        "calc_fifo_subtraction", // FIFO for subtraction
        "calc_fifo_multiplication", // FIFO for multiplication
        "calc_fifo_division" // FIFO for division
    };
    char *result_fifo = "calc_fifo_result"; // FIFO to collect results
    char *programs[] = {
        "./addition", "./subtraction", "./multiplication", "./division"
    };

    double num1, num2, result;

     // Create FIFOs for each operation
    for (int i = 0; i < 4; i++) {
        mkfifo(fifos[i], 0666); // Create FIFO with read-write permissions
    }
    mkfifo(result_fifo, 0666); // Create FIFO for the results

  // Start each child process for mathematical operations
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork(); // Create a new process
        if (pid == 0) { // Child process
            execlp(programs[i], programs[i], NULL); 
            perror("Exec failed"); // Error message if exec fails
            exit(1); // Exit the child process on failure
        }
    }

    // Open FIFOs for writing to the child processes
    int write_fds[4];
    for (int i = 0; i < 4; i++) {
        write_fds[i] = open(fifos[i], O_WRONLY); // Open FIFO in write mode
        
    }
    int read_fd = open(result_fifo, O_RDONLY); // Open result FIFO in read mode

    while (1) {
        printf("\nCalculator Menu:\n");
        printf("1 - Addition\n");
        printf("2 - Subtraction\n");
        printf("3 - Multiplication\n");
        printf("4 - Division\n");
        printf("5 - Exit\n");
        printf("Choose an option: ");
        int choice;
        if (scanf("%d", &choice) != 1) { // Read the user's choice
            printf("Invalid input! Please enter a number between 1 and 5.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        if (choice == 5) { // Exit condition
            printf("Exiting calculator...\n");
            break;
        }
        if (choice < 1 || choice > 4) { // Invalid input
            printf("Invalid choice. Please select a number between 1 and 4.\n");
            continue;
        }
        printf("Enter first number: ");
        if (scanf("%lf", &num1) != 1) { // Read the first number
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        printf("Enter second number: ");
        if (scanf("%lf", &num2) != 1) { // Read the second number
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

       // Write the numbers to the corresponding FIFO
       write(write_fds[choice - 1], &num1, sizeof(double));  
       write(write_fds[choice - 1], &num2, sizeof(double));
       
        // Read the result from the result FIFO
       read(read_fd, &result, sizeof(double));
       printf("Result: %lf\n", result); // Print the result
    }

   // Clean up: Close and unlink all FIFOs
    for (int i = 0; i < 4; i++) {
        close(write_fds[i]);
        unlink(fifos[i]);
    }
    close(read_fd);
    unlink(result_fifo);

    return 0;
}

