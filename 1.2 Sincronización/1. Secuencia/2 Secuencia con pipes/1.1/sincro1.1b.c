#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>

/* Secuencia ABABCABABCABABC*/

struct message
{
    char body[9];
};
typedef struct message tMessage;

#define SIZE_MSG sizeof(tMessage)
#define CANT_PROCESOS 3

int pipeA[2];
int pipeB[2];
int pipeC[2];

void letraA()
{
    tMessage mensaje;

    close(pipeA[1]);
    close(pipeB[0]);
    close(pipeC[0]);
    close(pipeC[1]);

    while (1)
    {
        read(pipeA[0], &mensaje, SIZE_MSG);
        read(pipeA[0], &mensaje, SIZE_MSG);
        printf("A");
        fflush(NULL);
        strcpy(mensaje.body, "enviando");
        write(pipeB[1], &mensaje, SIZE_MSG);
    }
    close(pipeA[0]);
    close(pipeB[1]);
}

void letraB()
{
    tMessage mensaje;

    close(pipeA[0]);
    close(pipeB[1]);
    close(pipeC[0]);

    while (1)
    {
        read(pipeB[0], &mensaje, SIZE_MSG);
        printf("B");
        fflush(NULL);
        strcpy(mensaje.body, "enviando");
        write(pipeA[1], &mensaje, SIZE_MSG);
        write(pipeC[1], &mensaje, SIZE_MSG);
    }
    close(pipeA[1]);
    close(pipeB[0]);
    close(pipeC[1]);
}

void letraC()
{
    tMessage mensaje;

    close(pipeA[0]);
    close(pipeB[0]);
    close(pipeB[1]);
    close(pipeC[1]);

    while (1)
    {
        read(pipeC[0], &mensaje, SIZE_MSG);
        read(pipeC[0], &mensaje, SIZE_MSG);
        printf("C\n");
        fflush(NULL);
        strcpy(mensaje.body, "enviando");
        write(pipeA[1], &mensaje, SIZE_MSG);
        write(pipeA[1], &mensaje, SIZE_MSG);
    }
    close(pipeC[0]);
    close(pipeA[1]);
}


int main(int argc, char *argv[])
{
    pid_t pidB, pidC;
    int* pipes[] = {pipeA, pipeB, pipeC};
    void* letras[] = {letraA, letraB, letraC};

    for(int i = 0; i < CANT_PROCESOS; i++)
    {
        //Creo pipes
        if(pipe(pipes[i]) == -1)
        {
            perror("Error al crea Pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    tMessage mensaje;
    strcpy(mensaje.body, "enviando");
    
    write(pipeA[1], &mensaje, SIZE_MSG);
    write(pipeA[1], &mensaje, SIZE_MSG);
    write(pipeA[1], &mensaje, SIZE_MSG);

    //Creo hijos
    pidB = fork();
    if (pidB < 0)
    {
        perror("Error fork B");
        exit(EXIT_FAILURE);
    }
    else if (pidB == 0)
    {
        letraB();
    }
    else
    { // padre
        // Creo hijo C
        pidC = fork();
        if (pidC < 0)
        {
            perror("Error fork C");
            exit(EXIT_FAILURE);
        }
        else if (pidC == 0)
        {
            letraC();
        }
        else
        { // padre
           letraA();        
        }
    }
    return EXIT_SUCCESS;
}