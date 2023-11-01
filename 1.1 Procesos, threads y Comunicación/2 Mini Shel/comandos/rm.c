#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv) {
    if (argc != 2) 
    {
        perror("\033[0;31mrm: argumento esperado, no encontrado.\n");
    } 
    else
    {
        if(remove(argv[1]) != 0) 
        {
            perror("\033[0;31mError en el comando rm");
        }
        else
        {
            printf("Se elimino correctamente\n");
        }
    }
    return EXIT_SUCCESS;
}
