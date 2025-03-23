#include <stdio.h> // Standard input/output functions
#include <stdlib.h> // General utility functions

int main(int argc, char *argv[]) {
    if (argc != 2) { // Check if the correct number of arguments is provided
        fprintf(stderr, "Error: Missing result argument\n");
        return 1;
    }

    // Open the results.txt file in append mode
    FILE *file = fopen("results.txt", "a");
    if (file == NULL) { // Check if the file opened successfully
        perror("Failed to open results.txt");
        return 1;
    }

    // Write the result to the file
    fprintf(file, "Result: %s\n", argv[1]);
    fclose(file); // Close the file after writing

    return 0;
}

