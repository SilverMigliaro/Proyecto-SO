#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/wait.h>

#define CANT_CLIENTES 75
#define MAX_COLA_ENTRADA 30
#define MAX_CLIENTE_COLA 15
#define EMPLEADO_CLIENTES 1
#define EMPLEADO_EMPRESA 2

#define COMUN 0
#define EMPRESA 1
#define POLITICO 2

#define TIPO_CLIENTE_COMUN 1L
#define TIPO_CLIENTE_EMPRESA 2L
#define TIPO_CLIENTE_POLITICO 3L
#define TIPO_ENTRADA 4L
#define TIPO_EMPLEADO_COMUN 5L
#define TIPO_EMPLEADO_EMPRESA 6L

#define KEY_ENTRADA 200
#define KEY_CLIENTE_COMUN 201
#define KEY_CLIENTE_EMPRESA 202
#define KEY_CLIENTE_POLITICO 203
#define KEY_EMPLEADO_COMUN 204
#define KEY_EMPLEADO_EMPRESA 205

struct Mensaje
{
    long tipo;
    char message[100];
};
typedef struct Mensaje msg;

void cliente_comun(int id);
void cliente_empresa(int id);
void cliente_politico(int id);

int cola_entrada, cola_cliente_comun, cola_empresa, cola_politico, cola_empleado_comun, cola_empleado_empresa;

int main()
{
    srand(time(NULL));

    cola_entrada = msgget((key_t)KEY_ENTRADA, IPC_CREAT | 0666);
    cola_cliente_comun = msgget((key_t)KEY_CLIENTE_COMUN, IPC_CREAT | 0666);
    cola_empresa = msgget((key_t)KEY_CLIENTE_EMPRESA, IPC_CREAT | 0666);
    cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, IPC_CREAT | 0666);
    cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, IPC_CREAT | 0666);
    cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, IPC_CREAT | 0666);

    msg mensaje_entrada;
    msg mensaje_cliente_comun;
    msg mensaje_cliente_empresa;
    msg mensaje_cliente_politico;
    msg mensaje_empleado_comun;
    msg mensaje_empleado_empresa;

    mensaje_entrada.tipo = TIPO_ENTRADA;
    mensaje_cliente_comun.tipo = TIPO_CLIENTE_COMUN;
    mensaje_cliente_empresa.tipo = TIPO_CLIENTE_EMPRESA;
    mensaje_cliente_politico.tipo = TIPO_CLIENTE_POLITICO;
    mensaje_empleado_comun.tipo = TIPO_EMPLEADO_COMUN;
    mensaje_empleado_empresa.tipo = TIPO_EMPLEADO_EMPRESA;

    msgsnd(cola_empleado_comun, &mensaje_empleado_comun, sizeof(mensaje_empleado_comun), 0);

    for (int i = 0; i < EMPLEADO_EMPRESA; i++)
    {
        msgsnd(cola_empleado_empresa, &mensaje_empleado_empresa, sizeof(mensaje_empleado_empresa), 0);
    }

    for (int i = 0; i < MAX_COLA_ENTRADA; i++)
    {
        msgsnd(cola_entrada, &mensaje_entrada, sizeof(mensaje_entrada), 0);
    }

    for (int i = 0; i < MAX_CLIENTE_COLA; i++)
    {
        msgsnd(cola_cliente_comun, &mensaje_cliente_comun, sizeof(mensaje_cliente_comun), 0);
        msgsnd(cola_empresa, &mensaje_cliente_empresa, sizeof(mensaje_cliente_empresa), 0);
        msgsnd(cola_politico, &mensaje_cliente_politico, sizeof(mensaje_cliente_politico), 0);
    }

    for (int i = 0; i < CANT_CLIENTES; i++)
    {
        int tipo = rand() % 3;

        switch (tipo)
        {
        case COMUN:
            if (fork() == 0)
            {
                cliente_comun(i);
                exit(0);
            }
            break;
        case EMPRESA:
            if (fork() == 0)
            {
                cliente_empresa(i);
                exit(0);
            }
            break;
        case POLITICO:
            if (fork() == 0)
            {
                cliente_politico(i);
                exit(0);
            }
            break;
        }
    }

    for (int i = 0; i < CANT_CLIENTES; i++)
    {
        wait(NULL);
    }

    msgctl(cola_entrada, IPC_RMID, NULL);
    msgctl(cola_cliente_comun, IPC_RMID, NULL);
    msgctl(cola_empresa, IPC_RMID, NULL);
    msgctl(cola_politico, IPC_RMID, NULL);
    msgctl(cola_empleado_comun, IPC_RMID, NULL);
    msgctl(cola_empleado_empresa, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}

void cliente_comun(int id)
{

    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_cliente_comun = msgget((key_t)KEY_CLIENTE_COMUN, 0666);
    int cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, 0666);

    msg msg_entrada, msg_cola, msg_empleado;

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) >= 0)
    {
        printf("ClienteComun %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        msgrcv(cola_cliente_comun, &msg_cola, sizeof(msg_cola), TIPO_CLIENTE_COMUN, 0);
        printf("ClienteComun %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClienteComun %d :: Ingreso en la cola de cliente Comun\n", id);
        sleep(3);
        msgrcv(cola_empleado_comun, &msg_empleado, sizeof(msg_empleado), TIPO_EMPLEADO_COMUN, 0);
        printf("ClienteComun %d :: Se retira de la cola de cliente Comun para ser atendido\n", id);
        msgsnd(cola_cliente_comun, &msg_cola, sizeof(msg_cola), 0);
        printf("ClienteComun %d :: Esta siendo atendido por un empleado Comun\n", id);
        sleep(2);
        printf("ClienteComun %d :: Fue atendido por un empleado Comun\n", id);
        msgsnd(cola_empleado_comun, &msg_empleado, sizeof(msg_empleado), 0);
        printf("ClienteComun %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else
    {
        printf("ClienteComun %d :: No hay lugar en la cola de entrada, y se retira del banco\n", id);
    }
}

void cliente_empresa(int id)
{

    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_empresa = msgget((key_t)KEY_CLIENTE_EMPRESA, 0666);
    int cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, 0666);

    msg msg_entrada, msg_cola, msg_empleado;

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) >= 0)
    {
        printf("ClienteEmpresa %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        msgrcv(cola_empresa, &msg_cola, sizeof(msg_cola), TIPO_CLIENTE_EMPRESA, 0);
        printf("ClienteEmpleado %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClienteEmpeado %d :: Ingreso en la cola de cliente Empleado\n", id);
        sleep(3);
        msgrcv(cola_empleado_empresa, &msg_empleado, sizeof(msg_empleado), TIPO_EMPLEADO_EMPRESA, 0);
        printf("ClienteEmpresa %d :: Se retira de la cola de cliente Empresa para ser atendido\n", id);
        msgsnd(cola_empresa, &msg_cola, sizeof(msg_cola), 0);
        printf("ClienteEmpresa %d :: Esta siendo atendido por un empleado Empresa\n", id);
        sleep(2);
        printf("ClienteEmpresa %d :: Fue atendido por un empleado Empresa\n", id);
        msgsnd(cola_empleado_empresa, &msg_empleado, sizeof(msg_empleado), 0);
        printf("ClienteEmpresa %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else
    {
        printf("ClienteEmpresa %d :: No hay lugar en la cola de entrada, y se retira de banco\n", id);
    }
}

void cliente_politico(int id)
{

    int atendido = 1;
    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, 0666);
    int cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, 0666);
    int cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, 0666);

    msg msg_entrada, msg_cola, msg_empleado;

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) >= 0)
    {
        printf("ClientePolitico %d :: Espera en la cola de entrada\n", id);
        sleep(5);
        msgrcv(cola_politico, &msg_cola, sizeof(msg_cola), TIPO_CLIENTE_POLITICO, 0);
        printf("ClientePolitico %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClientePolitico %d :: Ingreso en la cola de cliente Politico\n", id);
        sleep(3);
        while (atendido)
        {
            if (msgrcv(cola_empleado_comun, &msg_empleado, sizeof(msg_empleado), TIPO_EMPLEADO_COMUN, IPC_NOWAIT) >= 0)
            {
                printf("ClientePolitico %d :: Se retira de la cola de cliente Politico\n", id);
                msgsnd(cola_politico, &msg_cola, sizeof(msg_cola), 0);
                printf("ClientePolitico %d :: Esta siendo atendido por un empleado Comun\n", id);
                sleep(2);
                printf("ClientePolitico %d :: Fue atendido por un empleado Comun\n", id);
                msgsnd(cola_empleado_comun, &msg_empleado, sizeof(msg_empleado), 0);
                printf("ClientePolitico %d :: Se retira del banco\n", id);
                atendido = 0;
            }
            else
            {
                if (msgrcv(cola_empleado_empresa, &msg_empleado, sizeof(msg_empleado), TIPO_EMPLEADO_EMPRESA, IPC_NOWAIT) >= 0)
                {
                    printf("ClientePolitico %d :: Se retira de la cola de cliente Politico\n", id);
                    msgsnd(cola_politico, &msg_cola, sizeof(msg_cola), 0);
                    printf("ClientePolitico %d :: Esta siendo atendido por un empleado Empresa\n", id);
                    sleep(5);
                    printf("ClientePolitico %d :: Fue atendido por un empleado Empresa\n", id);
                    msgsnd(cola_empleado_empresa, &msg_empleado, sizeof(msg_empleado), 0);
                    printf("ClientePolitico %d :: Se retira del banco\n", id);
                    atendido = 0;
                }
            }
        }
        fflush(NULL);
    }
    else
    {
        printf("ClientePolitico %d :: No hay lugar en la cola de entrada, y se retira del banco\n", id);
    }
}
