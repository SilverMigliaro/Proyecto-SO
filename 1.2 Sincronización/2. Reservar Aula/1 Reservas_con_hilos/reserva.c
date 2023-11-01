#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NUM_ALUMNOS 25
#define HORAS_DIA 12

struct Reserva {
    bool estado;
    int id;
}; 
typedef struct Reserva tReserva;

struct Alumno {
    int id;
};
typedef struct Alumno tAlumno;

tReserva reservas[HORAS_DIA];
pthread_mutex_t mutex;

void * alumno(void* arg);
void reservar(tAlumno* alumno);
void cancelar(tAlumno* alumno);
void consultar(tAlumno* alumno);

int main() {

    srand(time(NULL));
    tReserva reserva;
    reserva.estado = false;
    reserva.id = -1;

    for (int i = 0; i < HORAS_DIA; i++) {
        reservas[i] = reserva;
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t alumnos[NUM_ALUMNOS];
    tAlumno datos_alumnos[NUM_ALUMNOS];

    for (int i = 0; i < NUM_ALUMNOS; i++) {
        datos_alumnos[i].id = i + 1;
        pthread_create(&alumnos[i], NULL, alumno, &datos_alumnos[i]);
    }

    for (int i = 0; i < NUM_ALUMNOS; i++) {
        pthread_join(alumnos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

void reservar(tAlumno* alumno) {
    int hora = rand() % HORAS_DIA;
    pthread_mutex_lock(&mutex);
    if (!reservas[hora].estado) {
        reservas[hora].estado = true;
        reservas[hora].id = alumno->id;
        printf("Alumno %d :: reservó el aula a las %d:00hs\n", alumno->id, hora + 9);
    } else {
        printf("Alumno %d :: no pudo reservar el aula a las %d:00hs por que ya estaba reservada\n", alumno->id, hora + 9);
    }
    pthread_mutex_unlock(&mutex);
}

void cancelar(tAlumno* alumno) {
    int hora = rand() % HORAS_DIA;
    pthread_mutex_lock(&mutex);
    if (reservas[hora].estado){
        if(reservas[hora].id == alumno->id) {
            reservas[hora].estado = false;
            reservas[hora].id = -1;
            printf("Alumno %d :: canceló la reserva del aula a las %d:00hs\n", alumno->id, hora + 9);
        }
        else
        {
            printf("Alumno %d :: no pudo cancelar la reserva del aula a las %d:00hs por que estaba reservada por otro alumno\n", alumno->id, hora + 9);
        }
    } else 
    {
        printf("Alumno %d :: no pudo cancelar la reserva del aula a las %d:00hs por que el aula esta libre\n", alumno->id, hora + 9);
    }
    pthread_mutex_unlock(&mutex);
}

void consultar(tAlumno* alumno) {
    int hora = rand() % HORAS_DIA;
    if (reservas[hora].estado) {
        printf("Alumno %d consulta que el aula está reservada a las %d:00hs por el alumno %d\n", alumno->id, hora + 9,reservas[hora].id);
    } else {
        printf("Alumno %d consulta que el aula está libre a las %d:00hs\n", alumno->id, hora + 9);
    }
}

void * alumno(void* arg) {
    struct Alumno* alumno = (struct Alumno*)arg;
    for (int i = 0; i < 4; i++) {
        int operacion = rand() % 100;
        if (operacion < 50) {
            reservar(alumno);
        } else if (operacion < 75) {
            cancelar(alumno);
        } else {
            consultar(alumno);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}