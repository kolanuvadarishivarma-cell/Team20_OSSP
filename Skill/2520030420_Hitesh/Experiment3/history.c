#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_HISTORY 100
#define MAX_BUFFER 1024

char *history[MAX_HISTORY];
int history_count = 0;

/* Enable character-by-character input */
void enable_raw_mode(struct termios *old)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, old);
    raw = *old;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* Restore normal terminal input */
void disable_raw_mode(struct termios *old)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, old);
}

/* Add command to history */
void add_history(const char *command)
{
    if (strlen(command) == 0)
        return;

    if (history_count < MAX_HISTORY)
    {
        history[history_count] = strdup(command);
        history_count++;
    }
    else
    {
        free(history[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
            history[i - 1] = history[i];

        history[MAX_HISTORY - 1] = strdup(command);
    }
}

/* Clear current input from terminal */
void clear_line(int length)
{
    printf("\r");

    for (int i = 0; i < length; i++)
        printf(" ");

    printf("\r");
    fflush(stdout);
}

/* Print recalled command */
void show_command(const char *command)
{
    printf("\r> %s", command);
    fflush(stdout);
}

/* Free allocated memory */
void free_history(void)
{
    for (int i = 0; i < history_count; i++)
        free(history[i]);
}

int main()
{
    struct termios old_terminal;

    char buffer[MAX_BUFFER];
    int buffer_length = 0;

    int history_index = history_count;

    enable_raw_mode(&old_terminal);

    printf("Simple Shell History\n");
    printf("Type commands. Use UP/DOWN arrows for history.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1)
    {
        printf("> ");
        fflush(stdout);

        buffer_length = 0;
        buffer[0] = '\0';

        history_index = history_count;

        while (1)
        {
            char c;

            if (read(STDIN_FILENO, &c, 1) != 1)
                continue;

            /* ENTER */
            if (c == '\n' || c == '\r')
            {
                buffer[buffer_length] = '\0';

                printf("\n");

                if (strcmp(buffer, "exit") == 0)
                {
                    disable_raw_mode(&old_terminal);
                    free_history();

                    printf("Shell terminated.\n");
                    return 0;
                }

                if (buffer_length > 0)
                {
                    add_history(buffer);
                    printf("Command entered: %s\n", buffer);
                }

                break;
            }

            /* BACKSPACE */
            else if (c == 127 || c == 8)
            {
                if (buffer_length > 0)
                {
                    buffer_length--;
                    buffer[buffer_length] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }
            }

            /* ESCAPE SEQUENCE */
            else if (c == 27)
            {
                char seq[2];

                if (read(STDIN_FILENO, &seq[0], 1) != 1)
                    continue;

                if (read(STDIN_FILENO, &seq[1], 1) != 1)
                    continue;

                /* UP ARROW: ESC [ A */
                if (seq[0] == '[' && seq[1] == 'A')
                {
                    if (history_count == 0)
                        continue;

                    if (history_index > 0)
                        history_index--;

                    clear_line(buffer_length + 2);

                    strcpy(buffer, history[history_index]);
                    buffer_length = strlen(buffer);

                    show_command(buffer);
                }

                /* DOWN ARROW: ESC [ B */
                else if (seq[0] == '[' && seq[1] == 'B')
                {
                    if (history_count == 0)
                        continue;

                    if (history_index < history_count - 1)
                    {
                        history_index++;

                        clear_line(buffer_length + 2);

                        strcpy(buffer, history[history_index]);
                        buffer_length = strlen(buffer);

                        show_command(buffer);
                    }
                    else
                    {
                        history_index = history_count;

                        clear_line(buffer_length + 2);

                        buffer[0] = '\0';
                        buffer_length = 0;
                    }
                }
            }

            /* NORMAL CHARACTER */
            else if (buffer_length < MAX_BUFFER - 1)
            {
                buffer[buffer_length++] = c;
                buffer[buffer_length] = '\0';

                putchar(c);
                fflush(stdout);
            }
        }
    }

    disable_raw_mode(&old_terminal);
    free_history();

    return 0;
}
