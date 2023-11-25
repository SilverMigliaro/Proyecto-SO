#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* Secuencia ABABCABABCABABC*/

#define CANT_PROCESOS 3

sem_t sem_a;
sem_t sem_b;
sem_t sem_c;

void* letraA();
void* letraB();
void* letraC();

int main() {

    sem_init(&sem_a, 0, 3); 
    sem_init(&sem_b, 0, 0);   
    sem_init(&sem_c, 0, 0);

    void* letras[] = {letraA, letraB, letraC};
    pthread_t hilos[CANT_PROCESOS];

    for (int i = 0; i < CANT_PROCESOS; i++) {
        pthread_create(&hilos[i], NULL, (void*)(letras[i]), NULL);
    }

    for (int i = 0; i < CANT_PROCESOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);

    return EXIT_SUCCESS;
}

void* letraA(){
    while(1){
        sem_wait(&sem_a);
        sem_wait(&sem_a);
        printf("A");
        fflush(NULL);
        sem_post(&sem_b);
    }
    pthread_exit(NULL); 
}

void* letraB(){
    while(1){
        sem_wait(&sem_b);
        printf("B");
        fflush(NULL);
        sem_post(&sem_a);
        sem_post(&sem_c);
    }
    pthread_exit(NULL); 
}

void* letraC(){
    while(1){
        sem_wait(&sem_c);
        sem_wait(&sem_c);
        printf("C\n");
        fflush(NULL);
        sem_post(&sem_a);
        sem_post(&sem_a);
    }
    pthread_exit(NULL); 
}