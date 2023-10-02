#ifndef __IMPLEMENTACION_COMANDOS__
#define __IMPLEMENTACION_COMANDOS__

/**
*Retorna la cantidad de comandos que soporta la shell.
*/
int contador_funciones();

extern char *lista_de_comandos[];
extern int (*lista_de_funciones[])(char**);

#endif // __IMPLEMENTACION_COMANDOS__
