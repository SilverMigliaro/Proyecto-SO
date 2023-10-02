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
 * Modifica los permisos de un archivo.
 */
int command_chmod(char **args) 
{
        int opt, aux;
        if (args[1] == NULL) 
        {
            perror("chmod: argumento esperado, null encontrado.\n");
        } 
        else 
        {
            opt = strtol(args[1], 0, 8); // cambia a base 8
            if (chmod(args[2], opt) != 0) 
            {
                perror("chmod: Error al ingresar en la direccion indicada.");
            }
        }
        return EXIT_SUCCESS;
}
