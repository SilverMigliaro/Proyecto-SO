#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define CANT_FUNCIONES 8
#define DELIMITADORES " \t\r\n\a"

char *lista_de_comandos[] = {
    "mkdir",
    "rm",
    "touch",
    "ls",
    "cat",
    "chmod",
    "help",
    "clear"
};

void ejecutar(char **args)
{
    pid_t pid;
    int encontre = 0;
    char comando[64] = "./comandos/";

    for (int i = 0; i < CANT_FUNCIONES && !encontre; i++) {
        if (strcmp(args[0], lista_de_comandos[i]) == 0) {
            strcat(comando, args[0]);
            encontre = 1;
        }
    }

    if (encontre) {
        pid = fork();
        if (pid == 0) {
            if (execv(comando, args) == -1)
                perror("\033[0;31mError al ejecutar el comando");
        } else {
            if (pid < 0) {
                perror("\033[0;31mError fork :: Error al lanzar el metodo");
            } else {
                wait(NULL);
            }
        }
    } else {
        printf("\033[0;31mMiniShell ~$ El comando ingresado no es valido\n");
        printf("\033[0;33mMiniShell ~$ Si necesita ayuda, consulte el comando \033[0;36mhelp\n");
    }
}

char *get_comando()
{
    char *entrada = (char *)malloc(32);
    int caracter;
    int leer = 1;
    int i = 0;

    while (leer) {
        caracter = getchar();
        if (caracter == '\n') {
            entrada[i] = '\0';
            leer = 0;
        } else {
            entrada[i] = caracter;
            i++;
        }
    }
    return entrada;
}

void separar_str(char *linea, char **args)
{
    int i = 0;
    char *argumento = strtok(linea, DELIMITADORES);
    while (argumento != NULL && i < 5) {
        args[i] = argumento;
        i++;
        argumento = strtok(NULL, DELIMITADORES);
    }
    args[i] = NULL; // Terminar la lista de argumentos con NULL
}

int main(int argc, char *argv[])
{
    char *linea;
    char *args[6]; // Cambiado de char **args a char *args[6]
    int estado = 1;

    printf("Comisión 2\nIntegrantes:: Francisco Ruiz Gómez, Silvestre Migliaro\nSistemas operativos 2023\n");
    sleep(3);
    write(STDOUT_FILENO, "\33[H\33[2J", 7);

    while (estado) {
        printf("\033[0;32mMiniShell ~$\033[0m ");
        linea = get_comando();

        if (strcmp(linea, "exit") == 0) {
            estado = 0;
        } else {
            separar_str(linea, args);
            ejecutar(args);
        }

        free(linea); // Liberar la memoria asignada a entrada
    }

    printf("Saliendo de MiniShell...\n");

    return EXIT_SUCCESS;
}
