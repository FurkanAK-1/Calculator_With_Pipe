#include <stdio.h> // Standard input/output functions
#include <stdlib.h> // General utility functions
#include <unistd.h> // POSIX API for fork, exec, etc.
#include <fcntl.h> // File control options

int main() {
    char *divison_fifo = "calc_fifo_division"; // FIFO for division input
    char *result_fifo = "calc_fifo_result"; // FIFO for sending results

    double num1, num2, result;

    int read_fd = open(divison_fifo, O_RDONLY); // Open FIFO for reading
    int write_fd = open(result_fifo, O_WRONLY); // Open FIFO for writing
   
    while (1) {

        // Read the first number from the FIFO
        if (read(read_fd, &num1, sizeof(double)) <= 0) {
            break;  // Exit loop if read fails
        }
        
        // Read the second number from the FIFO
        if (read(read_fd, &num2, sizeof(double)) <= 0) {
            break;  // Exit loop if read fails
        }

        // Check for division by zero
        if (num2 == 0) {
        printf("Error: Division by zero\n"); // Print error message
        result=0;
        write(write_fd, &result, sizeof(double)); // Write the result to the result FIFO
        continue; // Skip to the next iteration
        
    }

    // Perform the division
    result = num1 / num2;


        // Write the result to the result FIFO
        char result_str[50];
        sprintf(result_str, "%lf", result); // Convert result to string
        if (fork() == 0) {  // Fork a new process for saver
            execlp("./saver", "./saver", result_str, NULL); // Execute saver program
            perror("Exec failed");
            exit(1);
        }

        // Write the result to the FIFO
        write(write_fd, &result, sizeof(double));
    }

    close(read_fd); // Close the read FIFO
    close(write_fd); // Close the write FIFO
    return 0;
}

