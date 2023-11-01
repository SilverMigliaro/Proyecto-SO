#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char ** argv) {
    printf("\033[0;33m+------------------------- MiniShell de comandos ---------------------------+\n");
    printf("| Proyecto Sistemas Operativos 2023                                         |\n");
    printf("| Integrantes: Fransico Ruiz Gómez, Silvestre Migliaro.                         |\n");
    printf("+---------------------------------------------------------------------------+\n");

    if (argc == 1) {
        printf("Los comandos soportados son los siguientes:\n");
        printf("Comandos disponibles: \n");
        printf("'cat'\n");
        printf("'chmod'\n");
        printf("'clear'\n");
        printf("'ls'\n");
        printf("'mkdir'\n");
        printf("'rm'\n");
        printf("'touch'\n");
    } 
    else{
        if (argc == 2){
            if(strcmp(argv[1],"cat") == 0){
                printf("'cat [nombre de archivo]': Muestra el contenido de un archivo\n");
            }else if(strcmp(argv[1],"chmod") == 0){
                printf("'chmod [permisos] [nombre de archivo] ': Modifica los permisos de un archivo\n");
                printf("[permisos]: 111...777\n");
                printf("permisos de propietario - permisos de grupos - permisos de otros\n");
                printf("1: Permisos de ejecucion\n");
                printf("2: Permisos de escritura\n");
                printf("4: Permisos de lectura\n");
                printf("La suma de 1-2-4 permite la combinacion de estos permisos para el grupo/propietario/otros\n");
            }else if(strcmp(argv[1],"clear") == 0){
                printf("'clear': Limpia la consola\n");
            }else if(strcmp(argv[1],"ls") == 0){
                printf("'ls': Listar el contenido del directorio actual\n");
            } else if(strcmp(argv[1],"mkdir") == 0){
                printf("'mkdir [nombre de directorio]': Crear un directorio si no existe\n");
            } else if(strcmp(argv[1],"rm") == 0){
                printf("'rm [nombre de directorio]': Remover un directorio si existe\n");
            } else if(strcmp(argv[1],"touch") == 0){
                printf("'touch [nombre de archivo]': Crear un nuevo archivo si no existe\n");
            }  
        }else {
            printf("Error-%s: Cantidad de parametros invalida\n", argv[1]);    
        }
    }
    exit(EXIT_FAILURE);
}
