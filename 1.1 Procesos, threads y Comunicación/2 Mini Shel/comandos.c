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
#include "comandos.h"
#include "comandos/cat.h"
#include "comandos/cd.h"
#include "comandos/clear.h"
#include "comandos/ls.h"
#include "comandos/mkdir.h"
#include "comandos/rm.h"
#include "comandos/touch.h"
#include "comandos/chmod.h"
#include "comandos/help.h"

char *lista_de_comandos[] = {
    "mkdir",
    "rm",
    "touch",
    "ls",
    "cd",
    "cat",
    "chmod",
    "cd",
    "help",
    "clear",
    "exit"   
};


int (*lista_de_funciones[]) (char**) = {
    &command_mkdir,
    &command_rm,
    &command_touch,
    &command_ls,
    &command_cd,
    &command_cat,
    &command_chmod,
    &command_cd,
    &command_help,
    &command_clear
};


/**
 * Retorna la cantidad de comandos que soporta la shell.
 */
int contador_funciones() {
    return sizeof(lista_de_comandos) / sizeof(char *);
}
