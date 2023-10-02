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
#include "../comandos.h"

/**
 *  Imprime por stdout ayuda para el usuario.
 */
int command_help() {
    int i;
    printf("+------------------------- MiniShell de comandos ---------------------------+\n");
    printf("| Proyecto Sistemas Operativos 2022                                         |\n");
    printf("| Integrantes: Schwerdt Matias, Silvestre Migliaro.                         |\n");
    printf("| Modo de uso: Escribir el comando deseado seguido de los parametros,       |\n");
    printf("| y a continuacion presionar enter.                                         |\n");
    printf("+---------------------------------------------------------------------------+\n");
    printf("Los comandos soportados son los siguientes:\n");

    for (i = 0; i < contador_funciones(); i++) {
        printf("  %s\n", lista_de_comandos[i]);
    }
    printf("Para mayor informacion revisar el man de linux.\n");
    return EXIT_SUCCESS;
}
