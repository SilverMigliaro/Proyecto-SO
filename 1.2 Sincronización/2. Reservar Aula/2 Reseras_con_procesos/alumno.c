#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>

#define CANT_ALUMNOS 25
#define CANT_HORAS 12

struct Reserva {
    bool estado;
    int id;
}; 
typedef struct Reserva tReserva;

struct Memoria{
    tReserva reservas[CANT_HORAS];
    sem_t mutex;
};
typedef struct Memoria tMemoria;

#define KEY ((key_t)(1234))
#define SHM_SIZE sizeof(tMemoria)

void reservar(int id, tMemoria *  memoria);
void cancelar(int id, tMemoria *  memoria);
void consultar(int id, tMemoria *  memoria);

int main() {

    srand(getpid());

    int id_memoria;

	id_memoria = shmget (KEY, SHM_SIZE, 0666);
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

    for (int i = 0; i < 4; i++) {
        int operacion = rand() % 100;
        int id = getpid()-getppid();
        if (operacion < 50) {
            reservar(id,memoria);
        } else if (operacion < 75) {
            cancelar(id,memoria);
        } else {
            consultar(id,memoria);
        }
        sleep(1);
    }

    shmdt(memoria);

    return EXIT_SUCCESS;
}

void reservar(int id, tMemoria *  memoria) {

    int hora = rand() % CANT_HORAS;
    sem_wait(&(memoria->mutex));
    if (!memoria->reservas[hora].estado) {
        memoria->reservas[hora].estado = true;
        memoria->reservas[hora].id = id;
        printf("Alumno %d :: reservó el aula a las %d:00hs\n", id, hora + 9);
    } else {
        printf("Alumno %d :: no pudo reservar el aula a las %d:00hs por que ya estaba reservada\n", id, hora + 9);
    }
    sem_post(&(memoria->mutex));
}

void cancelar(int id, tMemoria *  memoria) {

    int hora = rand() % CANT_HORAS;
    sem_wait(&(memoria->mutex));
    if (memoria->reservas[hora].estado){
        if(memoria->reservas[hora].id == id) {
            memoria->reservas[hora].estado = false;
            memoria->reservas[hora].id = -1;
            printf("Alumno %d :: canceló la reserva del aula a las %d:00hs\n", id, hora + 9);
        }
        else
        {
            printf("Alumno %d :: no pudo cancelar la reserva del aula a las %d:00hs por que estaba reservada por otro alumno\n",id, hora + 9);
        }
    } else 
    {
        printf("Alumno %d :: no pudo cancelar la reserva del aula a las %d:00hs por que el aula esta libre\n",id, hora + 9);
    }
    sem_post(&(memoria->mutex));
}

void consultar(int id, tMemoria *  memoria) {

    int hora = rand() % CANT_HORAS;
    if (memoria->reservas[hora].estado) {
        printf("Alumno %d consulta que el aula está reservada a las %d:00hs por el alumno %d\n", id, hora + 9,memoria->reservas[hora].id);
    } else {
        printf("Alumno %d consulta que el aula está libre a las %d:00hs\n",id, hora + 9);
    }
}
