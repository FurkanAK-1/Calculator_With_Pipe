#include <stdio.h> // Standard input/output functions
#include <stdlib.h> // General utility functions
#include <unistd.h> // POSIX API for fork, exec, etc.
#include <fcntl.h> // File control options

int main() {
    char *multiplication_fifo = "calc_fifo_multiplication"; // FIFO for multiplication input
    char *result_fifo = "calc_fifo_result"; // FIFO for sending results

    double num1, num2, result;

    // Open FIFOs for reading and writing
    int read_fd = open(multiplication_fifo, O_RDONLY); // Open FIFO for reading numbers
    int write_fd = open(result_fifo, O_WRONLY); // Open FIFO for writing results

   
    while (1) {
        
        // Read the first number from the FIFO
        if (read(read_fd, &num1, sizeof(double)) <= 0) {
            break; // Exit loop if read fails
        }

        // Read the second number from the FIFO
        if (read(read_fd, &num2, sizeof(double)) <= 0) {
            break;  // Exit loop if read fails
        }

        // Perform the multiplication
        result = num1 * num2;

        // Send the result to the saver program
        char result_str[50];
        sprintf(result_str, "%lf", result); // Convert result to string
        if (fork() == 0) {  // Fork a new process for saver
            execlp("./saver", "./saver", result_str, NULL); // Execute saver program
            perror("Exec failed");
            exit(1);
        }

        // Write the result to the result FIFO
        write(write_fd, &result, sizeof(double));
    }

    close(read_fd); // Close the read FIFO
    close(write_fd); // Close the write FIFO
    return 0;
}

