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
int main(int argc, char ** argv){
    if(argc != 2){
        perror("\033[0;31mcat: argumento esperado, null encontrado.\n");
    }
    else{
        int file = open(argv[1], O_RDONLY);
        if (file == -1){
            perror("\033[0;31mcat: Error al abrir el archivo.\n");
        }
        else{
            int op;
            char c[128]; 
    
            while((op=read(file, c, 128))){
                write(STDOUT_FILENO, c, op);
            }
            printf("\n");
            close(file); 
        }
    }
    exit(EXIT_SUCCESS);
}
