#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    printf("child:\nВведите 1, чтобы завершить...\n");
    char c = getchar();
    if (c == '1') {
        kill(getpid(), SIGINT);
    } else {
        exit(1);
    }
    return 0;
}
