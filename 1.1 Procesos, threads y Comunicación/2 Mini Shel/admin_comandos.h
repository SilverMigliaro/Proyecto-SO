#ifndef __ADMINISTRACION_COMANDOS__
#define __ADMINISTRACION_COMANDOS__

/**
*Lee un comando de consola y lo retorna
*/
char *get_comando();

/**
*Separa la cadena enviada en str por los espacios ' ',
*devuelve los tokens en un arreglo de cadenas
*/
char **separar_str(char *str);

#endif // __ADMINSTRACION_COMANDOS__
