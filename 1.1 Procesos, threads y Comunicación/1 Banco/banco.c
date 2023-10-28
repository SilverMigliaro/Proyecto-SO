#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define CANT_CLIENTES 75
#define MAX_COLA_ENTRADA 30
#define MAX_CLIENTE_COLA 15
#define EMPLEADO_CLIENTES 1
#define EMPLEADO_EMPRESA 2

#define COMUN 0
#define EMPRESA 1
#define POLITICO 2

sem_t cola_entrada, cola_cliente_comun, cola_empresa, cola_politico, empleado_comun, empleado_empresa;

void *cliente_comun(void *args);
void *cliente_empresa(void *args);
void *cliente_politico(void *args);

int main()
{

    srand(time(NULL));

    pthread_t clientes[CANT_CLIENTES];
    int ids[CANT_CLIENTES];

    for (int i = 0; i < CANT_CLIENTES; i++)
    {
        ids[i] = i;
    }

    sem_init(&cola_cliente_comun, 1, MAX_CLIENTE_COLA);
    sem_init(&cola_empresa, 1, MAX_CLIENTE_COLA);
    sem_init(&cola_politico, 1, MAX_CLIENTE_COLA);
    sem_init(&cola_entrada, 1, MAX_COLA_ENTRADA);
    sem_init(&empleado_comun, 1, EMPLEADO_CLIENTES);
    sem_init(&empleado_empresa, 1, EMPLEADO_EMPRESA);

    for (int i = 0; i < CANT_CLIENTES; i++)
    {
        int tipo = rand() % 3;

        switch (tipo)
        {
        case COMUN:
            pthread_create(&(clientes[i]), NULL, cliente_comun, &ids[i]);
            break;
        case EMPRESA:
            pthread_create(&(clientes[i]), NULL, cliente_empresa, &ids[i]);
            break;
        case POLITICO:
            pthread_create(&(clientes[i]), NULL, cliente_politico, &ids[i]);
            break;
        }
    }

    for (int i = 0; i < CANT_CLIENTES; i++)
    {
        pthread_join(clientes[i], NULL);
    }

    sem_destroy(&cola_entrada);
    sem_destroy(&cola_cliente_comun);
    sem_destroy(&cola_empresa);
    sem_destroy(&cola_politico);
    sem_destroy(&empleado_comun);
    sem_destroy(&empleado_empresa);

    return EXIT_SUCCESS;
}

void *cliente_comun(void *args)
{

    int id = *((int *)args);

    if (sem_trywait(&cola_entrada) == 0)
    {
        printf("ClienteComun %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        sem_wait(&cola_cliente_comun);
        printf("ClienteComun %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClienteComun %d :: Ingreso en la cola de cliente Comun\n", id);
        sleep(3);
        sem_wait(&empleado_comun);
        printf("ClienteComun %d :: Se retira de la cola de cliente Comun para ser atendido\n", id);
        sem_post(&cola_cliente_comun);
        printf("ClienteComun %d :: Esta siendo atendido por un empleado Comun\n", id);
        sleep(1);
        printf("ClienteComun %d :: Fue atendido por un empleado Comun\n", id);
        sem_post(&empleado_comun);
        printf("ClienteComun %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else
    {
        printf("ClienteComun %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }

    pthread_exit(NULL);
}

void *cliente_empresa(void *args)
{

    int id = *((int *)args);

    if (sem_trywait(&cola_entrada) == 0)
    {
        printf("ClienteEmpresa %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        sem_wait(&cola_empresa);
        printf("ClienteEmpresa %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClienteEmpresa %d :: Ingreso en la cola de cliente Empresa\n", id);
        sleep(3);
        sem_wait(&empleado_empresa);
        printf("ClienteComun %d :: Se retira de la cola de cliente Comun para ser atendido\n", id);
        sem_post(&cola_cliente_comun);
        printf("ClienteEmpresa %d :: Esta siendo atendido por un empleado Empresa\n", id);
        sleep(1);
        printf("ClienteEmpresa %d :: Fue atendido por un empleado Empresa\n", id);
        sem_post(&empleado_empresa);
        printf("ClienteEmpresa %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else
    {
        printf("ClienteEmpresa %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }

    pthread_exit(NULL);
}

void *cliente_politico(void *args)
{

    int id = *((int *)args);
    int atendido = 1;

    if (sem_trywait(&cola_entrada) == 0)
    {
        printf("ClientePolitico %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        sem_wait(&cola_politico);
        printf("ClientePolitico %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClientePolitico %d :: Ingreso en la cola de cliente Politico\n", id);
        sleep(3);

        while (atendido)
        {
            if (sem_wait(&empleado_comun) == 0)
            {
                printf("ClientePolitico %d :: Se retira de la cola de cliente Politico para ser atendido\n", id);
                sem_post(&cola_politico);
                printf("ClientePolitico %d :: Esta siendo atendido por un empleado Comun\n", id);
                sleep(1);
                printf("ClientePolitico %d :: Fue atendido por un empleado Comun\n", id);
                sem_post(&empleado_comun);
                printf("ClientePolitico %d :: Se retira del banco\n", id);
                fflush(NULL);
                atendido = 0;
            }
            else
            {
                if (sem_wait(&empleado_empresa) == 0)
                {
                    printf("ClientePolitico %d :: Se retira de la cola de cliente Politico para ser atendido\n", id);
                    sem_post(&cola_politico);
                    printf("ClientePolitico %d :: Esta siendo atendido por un empleado Empresa\n", id);
                    sleep(1);
                    printf("ClientePolitico %d :: Fue atendido por un empleado Empresa\n", id);
                    sem_post(&empleado_empresa);
                    printf("ClientePolitico %d :: Se retira del banco\n", id);
                    fflush(NULL);
                    atendido = 0;
                }
            }
        }
    }
    else
    {
        printf("ClientePolitico %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }

    pthread_exit(NULL);
}
