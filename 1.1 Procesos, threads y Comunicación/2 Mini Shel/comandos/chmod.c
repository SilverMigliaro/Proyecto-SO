#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main (int argc, char ** argv) {
        if (argc != 3){
            perror("\033[0;31mchmod: argumento esperado, no encontrado.\n");
        } 
        else{
            if (access(argv[2], F_OK) == -1){
                perror("\033[0;31mchmod: Error al obtener información del archivo/directorio.\n");
            }
        
            char *endptr;        
            long opt = strtol(argv[1], &endptr, 8); 
             
            if (*endptr != '\0'){
                perror("\033[0;31mchmod: Los permisos deben ser un número octal válido.\n");
            }
            else{
            	if(opt>=0 && opt<=511){
            
                    if (chmod(argv[2], opt) != 0){
                        perror("\033[0;31mchmod: Error al ingresar en la direccion indicada.\n");
                    }
                    else{
                        printf("Permisos cambiados correctamente\n");
                    }
                }
                else{
                    perror("\033[0;31mchmod: codigo de permisos invalido.\n");
                }
             }   
        }   
      exit(EXIT_SUCCESS);
}
