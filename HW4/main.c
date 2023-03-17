#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int copy_file(const char *input_file_path, const char *output_file_path);

int main(int argc, char *argv[]) {
    // check if the number of arguments is correct
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // call the function to copy the file
    int status = copy_file(argv[1], argv[2]);
    if (status != 0) {
        printf("Error: failed to copy file.\n");
        return 1;
    }

    return 0;
}

// function to copy a file
int copy_file(const char *input_file_path, const char *output_file_path) {
    int input_file_descriptor, output_file_descriptor;
    ssize_t bytes_read, bytes_written;

    // open the input file for reading
    input_file_descriptor = open(input_file_path, O_RDONLY);
    if (input_file_descriptor == -1) {
        perror("Error: failed to open input file");
        return 1;
    }
    
    // get the status of the input file
    struct stat input_file_status;
    if (fstat(input_file_descriptor, &input_file_status) == -1) {
        perror("Error: failed to get input file status");
        close(input_file_descriptor);
        return 1;
    }
    
    // open the output file for writing, with the same permissions as the input file
    output_file_descriptor = open(output_file_path, O_WRONLY | O_CREAT | O_TRUNC, input_file_status.st_mode);
    if (output_file_descriptor == -1) {
        perror("Error: failed to create output file");
        close(input_file_descriptor);
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    // read from the input file and write to the output file until the end of the input file is reached
    while ((bytes_read = read(input_file_descriptor, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(output_file_descriptor, buffer, bytes_read);
        // check if all bytes were written to the output file
        if (bytes_written != bytes_read) {
            perror("Error: failed to write to output file");
            close(input_file_descriptor);
            close(output_file_descriptor);
            return 1;
        }
    }
    
    // close both files
    close(input_file_descriptor);
    close(output_file_descriptor);

    return 0;
}
