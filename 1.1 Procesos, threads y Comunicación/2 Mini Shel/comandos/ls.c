#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <utime.h>
#include <sys/stat.h>

/**
 * Imprime por stdout el contenido de el actual directorio.
 */
#define PATH_MAX_LENGTH 1024
/*
void listDirectory(const char *path) 
{
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        perror("\033[0;31mls: Error al abrir el directorio.");
        return;
    }

    struct dirent *entry;
    struct stat fileStat;
    char fullPath[PATH_MAX_LENGTH];

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue; // Omitir las entradas "." y ".."
        }

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
        if (stat(fullPath, &fileStat) == -1) 
        {
            perror("\033[0;31mls: Error al obtener información del archivo/directorio.");
            continue;
        }

        char sizeUnit[3];
        strcpy(sizeUnit, "B");
        off_t fileSize = fileStat.st_size;
        if (fileSize >= 1024) 
        {
            fileSize /= 1024;
            strcpy(sizeUnit, "KB");
        }
        
        char perms[11];
        snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c%c",
            (S_ISDIR(fileStat.st_mode)) ? 'd' : '-',
            (fileStat.st_mode & S_IRUSR) ? 'r' : '-',
            (fileStat.st_mode & S_IWUSR) ? 'w' : '-',
            (fileStat.st_mode & S_IXUSR) ? 'x' : '-',
            (fileStat.st_mode & S_IRGRP) ? 'r' : '-',
            (fileStat.st_mode & S_IWGRP) ? 'w' : '-',
            (fileStat.st_mode & S_IXGRP) ? 'x' : '-',
            (fileStat.st_mode & S_IROTH) ? 'r' : '-',
            (fileStat.st_mode & S_IWOTH) ? 'w' : '-',
            (fileStat.st_mode & S_IXOTH) ? 'x' : '-'
        );
         printf("%s %lld %s %s\n", perms, (long long)fileSize, sizeUnit, entry->d_name);
       } 

    closedir(dir);
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define PATH_MAX_LENGTH 1024

int main(int argc, char **argv) {

    DIR *directorio = opendir(".");
    
    if (directorio == NULL) {
        perror("\033[0;31mls: No se pudo acceder al directorio");
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada;
    entrada = readdir(directorio);
    struct stat fileStat;
    char fullPath[PATH_MAX_LENGTH];

    while (entrada != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            entrada = readdir(directorio);
            continue;
        }
        
        snprintf(fullPath, sizeof(fullPath), "%s/%s", ".", entrada->d_name);
        if (stat(fullPath, &fileStat) == -1) {
            perror("\033[0;31mls: Error al obtener información del archivo/directorio.");
            entrada = readdir(directorio);
            continue;
        }

        char sizeUnit[3];
        strcpy(sizeUnit, "B");
        off_t fileSize = fileStat.st_size;
        if (fileSize >= 1024) {
            fileSize /= 1024;
            strcpy(sizeUnit, "KB");
        }

        char perms[11];
        snprintf(perms, sizeof(perms), "%s%s%s%s%s%s%s%s%s%s",
            (S_ISDIR(fileStat.st_mode)) ? "d" : "-",
            (fileStat.st_mode & S_IRUSR) ? "r" : "-",
            (fileStat.st_mode & S_IWUSR) ? "w" : "-",
            (fileStat.st_mode & S_IXUSR) ? "x" : "-",
            (fileStat.st_mode & S_IRGRP) ? "r" : "-",
            (fileStat.st_mode & S_IWGRP) ? "w" : "-",
            (fileStat.st_mode & S_IXGRP) ? "x" : "-",
            (fileStat.st_mode & S_IROTH) ? "r" : "-",
            (fileStat.st_mode & S_IWOTH) ? "w" : "-",
            (fileStat.st_mode & S_IXOTH) ? "x" : "-"
        );

        printf("%s %lld %s %s\n", perms, (long long)fileSize, sizeUnit, entrada->d_name);
        entrada = readdir(directorio);
    }

    closedir(directorio);
    return EXIT_SUCCESS;
}

