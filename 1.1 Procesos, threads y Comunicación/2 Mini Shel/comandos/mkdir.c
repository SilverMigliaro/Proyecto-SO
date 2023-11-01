#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        perror("\033[0;31mmkdir: argumento esperado, no encontrado.\n");
    } 
    else
    {
        if (mkdir(argv[1], 0700) == 0) 
        {
            printf("Directorio '%s' creado con éxito.\n", argv[1]);
        } 
        else
        {
            perror("\033[0;31mmkdir: Error al crear el directorio");
        }
    }
    return EXIT_SUCCESS;
}
