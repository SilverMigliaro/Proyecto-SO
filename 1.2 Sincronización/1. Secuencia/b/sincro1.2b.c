#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>

/* Secuencia ABABCABCD*/

struct message
{
    char body[9];
};
typedef struct message tMessage;

#define SIZE_MSG sizeof(tMessage)
#define CANT_PROCESOS 4

int pipeA[2];
int pipeB[2];
int pipeC[2];
int pipeD[2];

void letraA()
{
    tMessage mensaje;

    close(pipeA[1]);
    close(pipeB[0]);
    close(pipeC[0]);
    close(pipeC[1]);
    close(pipeD[0]);
    close(pipeD[1]);

    while (1)
    {
        read(pipeA[0], &mensaje, SIZE_MSG);
        printf("A");
        fflush(NULL);
        sleep(1);
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
    close(pipeA[1]);
    close(pipeB[1]);
    close(pipeC[0]);
    close(pipeD[0]);
    close(pipeD[1]);

    while (1)
    {
        read(pipeB[0], &mensaje, SIZE_MSG);
        printf("B");
        fflush(NULL);
        sleep(1);
        strcpy(mensaje.body, "enviando");
        write(pipeC[1], &mensaje, SIZE_MSG);
    }
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
    close(pipeD[0]);

    while (1)
    {
        read(pipeC[0], &mensaje, SIZE_MSG);
        strcpy(mensaje.body, "enviando");
        write(pipeA[1], &mensaje, SIZE_MSG);
        read(pipeC[0], &mensaje, SIZE_MSG);
        printf("C");
        fflush(NULL);
        sleep(1);
        strcpy(mensaje.body, "enviando");
        write(pipeD[1], &mensaje, SIZE_MSG);
    }
    close(pipeC[0]);
    close(pipeA[1]);
    close(pipeD[1]);
}

void letraD()
{
    tMessage mensaje;

    close(pipeA[0]);
    close(pipeB[0]);
    close(pipeB[1]);
    close(pipeC[0]);
    close(pipeD[1]);

    while (1)
    {
        read(pipeD[0], &mensaje, SIZE_MSG);
        strcpy(mensaje.body, "enviando");
        write(pipeC[1], &mensaje, SIZE_MSG);
        read(pipeD[0], &mensaje, SIZE_MSG);
        printf("D\n");
        fflush(NULL);
        sleep(1);
        strcpy(mensaje.body, "enviando");
        write(pipeA[1], &mensaje, SIZE_MSG);
    }
    close(pipeC[0]);
    close(pipeA[1]);
    close(pipeD[1]);
}

int main(int argc, char *argv[])
{
    pid_t pidB, pidC, pidD;
    int* pipes[] = {pipeA, pipeB, pipeC, pipeD};

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
        // Creo hijo C
            pidD = fork();
            if (pidD < 0)
            {
                perror("Error fork D");
                exit(EXIT_FAILURE);
            }
            else if (pidD == 0)
            {
                letraD();
            }
            else{
                //padre
                letraA();   
            }
        }
    }
    return EXIT_SUCCESS;
}