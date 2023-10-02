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
 * Abre un archivo, y en caso de que no exista lo crea. Para luego
 * escribir en el la cadena de caracteres ingresada.
 */
int command_cat(char **args) {
    int file, op;
    char c[128]; 

    if (args[1] == NULL) {
        perror("cat: argumento esperado, null encontrado.\n");
    }
    file = open(args[1], O_RDONLY);
    if (file == -1)  {
        perror("cat: Error al abrir el archivo.\n");
    }
    while((op=read(file, c, 128))){
        write(STDOUT_FILENO, c, op);
    }
    printf("\n");
    close(file); 
    return EXIT_SUCCESS;
}