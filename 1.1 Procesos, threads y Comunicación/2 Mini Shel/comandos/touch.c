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
 * Crea un archivo, si este existe, borra todo su contenido.
 */
int command_touch(char ** args) {
    if (args[0] == NULL) {
        perror("touch: argumento esperado, null encontrado.\n");
    } else {
        FILE *file;
        file = fopen(args[1],"w");
        fclose(file);
    }
    return EXIT_SUCCESS;
}
