#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <utime.h>
#include <sys/stat.h>

/**
 * Crea un directorio en el path indicado (args[1]).
 */
int command_mkdir(char **args) {
    struct stat st;
    if (args[1] == NULL) {
        perror("mkdir: argumento esperado, null encontrado.\n");
    } else if (stat(args[1], &st) == -1) {
        mkdir(args[1], 0700);
    }
    return EXIT_SUCCESS;
}
