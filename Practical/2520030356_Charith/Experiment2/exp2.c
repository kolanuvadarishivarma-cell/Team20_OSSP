#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    printf("Parent process started\n");
    printf("Parent PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        // Child process
        printf("\n--- CHILD PROCESS ---\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        printf("Child is running...\n");

        sleep(10);

        printf("Child is terminating...\n");

        exit(0);
    }
    else
    {
        // Parent process
        printf("\n--- PARENT PROCESS ---\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);

        printf("Parent is waiting for child...\n");

        wait(NULL);

        printf("Child terminated.\n");
        printf("Parent process terminating.\n");
    }

    return 0;
}
