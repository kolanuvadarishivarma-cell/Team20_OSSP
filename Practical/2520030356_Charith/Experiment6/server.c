#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO1 "client_to_server"
#define FIFO2 "server_to_client"

int main() {
    char buffer[100];

    // Create FIFOs
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("Server started. Waiting for client...\n");

    // Open FIFOs
    int fd_read = open(FIFO1, O_RDONLY);
    int fd_write = open(FIFO2, O_WRONLY);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Read message from client
        int n = read(fd_read, buffer, sizeof(buffer) - 1);

        if (n <= 0) {
            break;
        }

        // Add null character
        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // Check for exit command
        if (strcmp(buffer, "exit\n") == 0 ||
            strcmp(buffer, "exit") == 0) {

            strcpy(buffer, "Server shutting down.");

            write(fd_write, buffer, strlen(buffer) + 1);

            break;
        }

        // Prepare response
        char response[100];

        snprintf(response, sizeof(response),
                 "Server received: %.80s", buffer);

        // Send response to client
        write(fd_write, response, strlen(response) + 1);
    }

    // Close FIFOs
    close(fd_read);
    close(fd_write);

    // Remove FIFOs
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
