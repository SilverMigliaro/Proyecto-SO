#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "comandos.h"
#include "admin_comandos.h"

/**
* Funcion que ejecuta un comando, creando un proceso hijo, 
* y este realiza la ejecucion.
*/
int ejecutar(char **args){

	pid_t pid;
	int i, wstatus;
	int (*funcion)(char**) = NULL;

	for(i = 0; i < contador_funciones(); i++){
		if(strcmp(args[0], lista_de_comandos[i]) == 0){
			funcion = &(*lista_de_funciones[i]);
		}
	}

	pid = fork();
	if(pid == 0){
		if(funcion == NULL){
			return execvp(args[0], args);
		} else {
			return (*funcion)(args);
		}
		exit(EXIT_FAILURE);
	} else if(pid < 0){
		perror("Error en el fork");
	} else {
		waitpid(pid, &wstatus, WUNTRACED);
	}
	return EXIT_FAILURE;

}

/**
* Funcion principal de la Shell, se reciben los comandos.
*/
int main(int argv, char *argc[]){
	char *linea;
	char **args;
	int estado;

	do{
		printf("MiniShell ~$ ");
		linea = get_comando();

        if (strcmp(linea,"exit") == 0){
            printf("Saliendo de MiniShell...\n");
            exit(EXIT_SUCCESS);
        }

		args = separar_str(linea);
		estado = ejecutar(args);
		free(linea);
		free(args);
	} while (estado);

	return EXIT_SUCCESS;
}
