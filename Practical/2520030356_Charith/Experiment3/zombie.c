#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process started\n");
        printf("Child PID = %d\n", getpid());

        printf("Child terminating...\n");
        exit(0);
    }
    else
    {
        printf("Parent process started\n");
        printf("Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent sleeping for 30 seconds...\n");

        sleep(30);

        printf("Parent terminating...\n");
    }

    return 0;
}
