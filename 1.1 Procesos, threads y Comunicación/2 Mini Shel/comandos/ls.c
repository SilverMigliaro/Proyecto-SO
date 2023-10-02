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
 * Imprime por stdout el contenido de el actual directorio.
 */
#define PATH_MAX_LENGTH 1024
int command_ls(char **args) {
    if (args[0] == NULL) {
        perror("ls: argumento esperado, null encontrado.\n");
        //fprintf(stderr, "ls: argumento esperado, null encontrado.\n");
    } else {
        DIR *dir;
        struct dirent *file;
        struct stat mstat;
        char currentDir[1024];
        char buf[512];

        if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
            perror("ls: getcurrentDir() error.\n");
            exit(EXIT_FAILURE);
        }
        dir = opendir(currentDir);
        if (dir == NULL) {
            perror("ls: Error al ejecutar el comando ls.\n");
            exit(EXIT_FAILURE);
        }
        while((file = readdir(dir)) != NULL) {
            sprintf(buf, "%s/%s", currentDir, file->d_name);
            stat(buf, &mstat);
            printf("%zu K", (mstat.st_size / 1000));
            printf(" %s\n", file->d_name);
        }   
        closedir(dir);
    }
    return EXIT_SUCCESS;
}
