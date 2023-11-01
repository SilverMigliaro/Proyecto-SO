#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    if (argc != 2) 
    {
        perror("\033[0;31mtouch: argumento esperado, no encontrado.\n");
    } 
    else
    {
        FILE *file = fopen(argv[1], "w");
        if (file == NULL)
        {
            perror("\033[0;31mtouch: Error al crear o actualizar el archivo");
        }
        else
        {
            printf("Se creo el archivo '%s' correctamente\n",argv[1]);
        }
        fclose(file);
    }
    return EXIT_SUCCESS;
}

