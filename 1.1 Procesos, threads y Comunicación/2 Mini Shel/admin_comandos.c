#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin_comandos.h"

//Lee un comando de consola y lo retorna.
#define RD_BUFFER 1024
char *get_comando() {
    int b_size = RD_BUFFER;
    int i = 0;
    char *buffer = malloc(sizeof(char) * b_size);
    int c;

    if (!buffer) {
        fprintf(stderr, "read: error al realocar memoria para leer\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[i] = '\0';
            return buffer;
        } else {
            buffer[i] = c;
        }
        i++;
        if (i >= b_size) {
            b_size += RD_BUFFER;
            buffer = realloc(buffer, b_size);
            if (!buffer) {
                fprintf(stderr, "read: error al realocar memoria para leer\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

//Delimita la cadena enviada en 'str' de acuerdo al delimitador ' ',
//devuelve los tokens en un arreglo de cadenas
#define TOKEN_b_size 8
char **separar_str(char *cadena) {
    int i = 0;
    int b_size = TOKEN_b_size;
    char *parsed;
    char **args = malloc(sizeof(char) * b_size);

    if (!args) {
        fprintf(stderr, "separar_str: Error de locacion de memoria.");
        exit(EXIT_FAILURE);
    }

    parsed = strtok(cadena, " ");
    while (parsed != NULL) {
        args[i++] = parsed;
        parsed = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}
