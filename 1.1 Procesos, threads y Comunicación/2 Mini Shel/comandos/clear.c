#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    write(STDOUT_FILENO, "\33[H\33[2J", 7);
    exit(EXIT_SUCCESS);
}
