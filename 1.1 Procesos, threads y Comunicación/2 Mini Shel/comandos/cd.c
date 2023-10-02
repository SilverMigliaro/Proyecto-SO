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
 * Ingresa en el directorio indicado.
 */
int command_cd(char **args) {
    if (args[0] == NULL) {
        perror("cd: argumento esperado, null encontrado.\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd: Error al ingresar en la direccion indicada.");
    }
    return EXIT_FAILURE;
}