#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    char input[100];

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nExiting...\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            continue;
        }

        if (pid == 0)
        {
            // Child process
            printf("Child PID: %d\n", getpid());

            char *args[20];
            int i = 0;

            char *token = strtok(input, " ");

            while (token != NULL && i < 19)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            execvp(args[0], args);

            perror("Command execution failed");
            exit(1);
        }
        else
        {
            // Parent process
            printf("Parent PID: %d\n", getpid());
            printf("Child PID: %d\n", pid);

            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
