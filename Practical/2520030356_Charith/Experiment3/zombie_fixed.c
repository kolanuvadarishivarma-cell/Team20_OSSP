#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process started\n");
        printf("Child PID = %d\n", getpid());

        sleep(2);

        printf("Child terminating...\n");
        exit(0);
    }
    else
    {
        printf("Parent process started\n");
        printf("Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        waitpid(pid, NULL, 0);

        printf("Child collected successfully.\n");
        printf("No zombie process remains.\n");
    }

    return 0;
}
