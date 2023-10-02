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
 * Remueve el archivo o directorio ingresado.
 */
int command_rm(char **args) {
    if (args[1] == NULL) {
        perror("rm: argumento esperado, null encontrado.\n");
    } else if (remove(args[1]) != 0) {
        perror("Error en el comando rm");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}