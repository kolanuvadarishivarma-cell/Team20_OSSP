#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Before exec()\n");

    execl("/bin/ls", "ls", "-l", NULL);

    printf("This line will only print if exec() fails.\n");

    return 0;
}
