#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>

#define CANT_ALUMNOS 25
#define CANT_HORAS 12
#define MAX_OPERACIONES 4

struct Reserva {
    bool estado;
    int id;
}; 
typedef struct Reserva tReserva;

struct Memoria{
    tReserva reservas[CANT_HORAS];
    sem_t mutex;
    sem_t escritor;
    sem_t lector;
};
typedef struct Memoria tMemoria;

#define KEY ((key_t)(1234))
#define SHM_SIZE sizeof(tMemoria)

int main() {
    
    int id_memoria;

	id_memoria = shmget (KEY, SHM_SIZE, IPC_CREAT | 0666);
	if (id_memoria == -1)
	{
		printf("No pude obtener el segmento de memoria \n");
		exit (1);
	}

	tMemoria *  memoria = NULL;
	memoria = (struct Memoria*) shmat (id_memoria, (char *)0, 0);

	if (memoria == NULL)
	{
		printf("No se pudo asignar la memoria compartida \n");
		exit (1);
	}

	sem_init(&(memoria->escritor),1,1);
    sem_init(&(memoria->lector),1,0);
    sem_init(&(memoria->mutex),1,1);
	
	tReserva reserva;
    reserva.estado = false;
    reserva.id = -1;

    for (int i = 0; i < CANT_HORAS; i++) {
        memoria->reservas[i] = reserva;
    }

    int id;

    for (int i = 0; i < CANT_ALUMNOS; i++)
    {
		id=fork();

		if(id == 0)
        {
		    execlp("./alumno", "alumno", "-alumno", NULL);
        }
    }
    
    sem_post(&(memoria->mutex));
	
	for (int i = 0; i < CANT_ALUMNOS; i++)
    {
        wait(NULL);
    }
    
    sem_destroy(&(memoria->mutex));
    shmdt(memoria);

    return EXIT_SUCCESS;
}

