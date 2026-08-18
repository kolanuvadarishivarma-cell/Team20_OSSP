#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char command[100];
    pid_t pid;

    printf("Enter a command: ");
    scanf("%99s", command);

    pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if (pid == 0) {
        printf("Child Process executing command...\n");

        execlp(command, command, NULL);

        perror("exec failed");
        exit(1);
    }
    else {
        printf("Parent Process\n");
        printf("Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        wait(NULL);

        printf("Child process completed.\n");
    }

    return 0;
}
