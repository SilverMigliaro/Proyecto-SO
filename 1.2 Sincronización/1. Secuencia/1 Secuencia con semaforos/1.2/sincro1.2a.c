#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define CANT_PROCESOS 4

sem_t sem_a;
sem_t sem_b;
sem_t sem_c;
sem_t sem_d;

void* letraA();
void* letraB();
void* letraC();
void* letraD();

int main() {

    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);
    sem_init(&sem_d, 0, 0);

    pthread_t hilos[CANT_PROCESOS];
    pthread_create(&hilos[0], NULL, letraA, NULL);
    pthread_create(&hilos[1], NULL, letraB, NULL);
    pthread_create(&hilos[2], NULL, letraC, NULL);
    pthread_create(&hilos[3], NULL, letraD, NULL);

    for (int i = 0; i < CANT_PROCESOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);
    sem_destroy(&sem_d);

    return EXIT_SUCCESS;
}

void* letraA(){
    while(1){
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
        sem_post(&sem_c);
    }
    pthread_exit(NULL);
}

void* letraC(){
    while(1){
        sem_wait(&sem_c);
        sem_post(&sem_a);
        sem_wait(&sem_c);
        printf("C");
        fflush(NULL);
        sem_post(&sem_d);
    }
    pthread_exit(NULL);
}

void* letraD(){
    while(1){
        sem_wait(&sem_d);
        sem_post(&sem_c);
        sem_wait(&sem_d);
        printf("D\n");
        sem_post(&sem_a);
    }
    pthread_exit(NULL);
}
