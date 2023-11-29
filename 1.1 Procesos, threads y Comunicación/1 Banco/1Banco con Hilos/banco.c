#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define CANT_CLIENTES 15
#define MAX_COLA_ENTRADA 8
#define MAX_CLIENTE_COLA 2
#define EMPLEADO_CLIENTES 1
#define EMPLEADO_EMPRESA 2

#define COMUN 0
#define EMPRESA 1
#define POLITICO 2

sem_t cola_entrada, 
    cola_cliente_comun,
    cola_politico,
    cola_empresa, 
    
    comun_esperando, 
    politico_esperando,
    empresa_esperando,

    empleado_comun, 
    empleado_empresa[EMPLEADO_EMPRESA],

    mutex_comun,
    mutex_empresa,
    mutex_politico,
    
    atencion_comun,
    atencion_empresa,
    atencion_politico;
    
void *cliente_comun(void *args){
    int id = *((int *)args);
	
    if (sem_trywait(&cola_entrada) == 0){
        printf("ClienteComun %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sem_wait(&cola_cliente_comun);
        sleep(1);
        printf("ClienteComun %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClienteComun %d :: Ingreso en la cola de cliente Comun\n", id);
        sem_post(&comun_esperando);
            
        if(sem_trywait(&empleado_comun) == -1 ){
            sem_post(&empleado_comun);
        }
        else{
            sem_post(&empleado_comun);
            sem_post(&empleado_comun);
        }
        
	    printf("ClienteComun %d :: Esperando se atendido por empleado cliente Comun\n", id);
	    sem_wait(&atencion_comun);
        printf("ClienteComun %d :: Se retira de la cola de cliente Comun para ser atendido\n", id);
        sem_wait(&mutex_comun);
        printf("ClienteComun %d :: Se retira del banco\n", id);
    }
    else{
        printf("ClienteComun %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }
    pthread_exit(NULL);
}

void *cliente_empresa(void *args){
	int id = *((int *)args);
	
    if (sem_trywait(&cola_entrada) == 0){
        printf("ClienteEmpresa %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sleep(1);
        sem_wait(&cola_empresa);
        printf("ClienteEmpresa %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClienteEmpresa %d :: Ingreso en la cola de cliente Empresa\n", id);
        sem_post(&empresa_esperando);

		if(sem_trywait(&empleado_empresa[0]) == -1 ){
		    sem_post(&empleado_empresa[0]);
		}
        else{
            sem_post(&empleado_empresa[0]);
            sem_post(&empleado_empresa[0]);
        }
		if(sem_trywait(&empleado_empresa[1]) == -1 ){
		    sem_post(&empleado_empresa[1]);
		}
        else{
            sem_post(&empleado_empresa[1]);
            sem_post(&empleado_empresa[1]);
        }
       	
		printf("ClienteEmpresa %d :: Esperando se atendido por empleado cliente Empresa\n", id);
		sem_wait(&atencion_empresa);
        printf("ClienteEmpresa %d :: Se retira de la cola de cliente Empresa para ser atendido\n", id);
        sem_wait(&mutex_empresa);
        printf("ClienteEmpresa %d :: Se retira del banco\n", id);
    }
    else{
        printf("ClienteEmpresa %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }
    pthread_exit(NULL);
}

void *cliente_politico(void *args){
	int id = *((int *)args);
	
    if (sem_trywait(&cola_entrada) == 0){
        printf("ClientePolitico %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sleep(1);
        sem_wait(&cola_politico);
        printf("ClientePolitico %d :: Se retira de la cola de entrada\n", id);
        sem_post(&cola_entrada);
        printf("ClientePolitico %d :: Ingreso en la cola de cliente Politico\n", id);
       	sem_post(&politico_esperando);
       		
        if(sem_trywait(&empleado_comun) == -1 ){
            sem_post(&empleado_comun);
        }
        else{
            sem_post(&empleado_comun);
            sem_post(&empleado_comun);
        }
        if(sem_trywait(&empleado_empresa[0]) == -1 ){
            sem_post(&empleado_empresa[0]);
        }
        else{
            sem_post(&empleado_empresa[0]);
            sem_post(&empleado_empresa[0]);
        }
        if(sem_trywait(&empleado_empresa[1]) == -1 ){
            sem_post(&empleado_empresa[1]);
        }
        else{
            sem_post(&empleado_empresa[1]);
            sem_post(&empleado_empresa[1]);
        }

	    printf("ClientePolitico %d :: Esperando se atendido por empleado del banco.\n", id);
	    sem_wait(&atencion_politico);
        printf("ClientePolitico %d :: Se retira de la cola de cliente Politico para ser atendido\n", id);
        sem_wait(&mutex_politico);
        printf("ClientePolitico %d :: Se retira del banco\n", id);
    }
    else{
        printf("ClientePolitico %d :: No hay lugar en la cola de entrada y se retira\n", id);
    }
    pthread_exit(NULL);
}

void *hilo_empleado_comun(void *args){
 	int id = *((int *)args);
 	int no_atendi = 0;
 	sem_wait(&empleado_comun);
 	
    while(no_atendi < 3){

        printf("\nEmpleadoComun %d :: Revisa si hay clientes politicos.\n", id);

        if(sem_trywait(&politico_esperando) == 0){
            printf("EmpleadoComun %d :: Estoy atendiendo un cliente politico.\n\n", id);
            sem_post(&atencion_politico);
            sem_post(&cola_politico);
            sem_post(&mutex_politico);
            sleep(5);
        }
        else{
            printf("EmpleadoComun %d :: Revisa si hay clientes comun.\n", id);
            
            if(sem_trywait(&comun_esperando) == 0){
                printf("EmpleadoComun %d :: Estoy atendiendo un cliente comun.\n\n", id);
                sem_post(&atencion_comun);
                sem_post(&cola_cliente_comun);
                sem_post(&mutex_comun);
                sleep(5);
            }
            else{
            	printf("EmpleadoComun %d :: No hay clientes por atender.\n\n", id);
    			sem_wait(&empleado_comun);
                no_atendi++;
            }
        }
    }
    pthread_exit(NULL);
}

void *hilo_empleado_empresa(void *args){
    int id = *((int *)args);
    int no_atendi = 0;
    sem_wait(&empleado_empresa[id]);

    while(no_atendi < 3){

        printf("\nEmpleadoEmpresa %d :: Revisa si hay clientes politicos.\n", id);

        if(sem_trywait(&politico_esperando) == 0){
            printf("EmpleadoEmpresa %d :: Estoy atendiendo un cliente politico.\n\n", id);
            sem_post(&atencion_politico);
            sem_post(&cola_politico);
            sleep(5);
            sem_post(&mutex_politico);
        }
        else{
            printf("EmpleadoEmpresa %d :: Revisa si hay clientes empresa.\n", id);
            if(sem_trywait(&empresa_esperando) == 0){
                printf("EmpleadoEmpresa %d :: Estoy atendiendo un cliente empresa.\n\n", id);
                sem_post(&atencion_empresa);
                sem_post(&cola_empresa);
                sleep(5);
                sem_post(&mutex_empresa);
            }
            else{
            	printf("EmpleadoEmpresa %d :: No hay clientes por atender.\n\n", id);
    			sem_wait(&empleado_empresa[id]);
                no_atendi++;
            }
        }
    }
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));

    pthread_t clientes[CANT_CLIENTES];
    pthread_t tEmpleado_empresa[EMPLEADO_EMPRESA];
    pthread_t tEmpleado_comun;

    int ids[CANT_CLIENTES];

    for (int i = 0; i < CANT_CLIENTES; i++){
        ids[i] = i;
    }

    sem_init(&cola_cliente_comun, 1, MAX_CLIENTE_COLA);
    sem_init(&comun_esperando, 1, 0);
    sem_init(&cola_empresa, 1, MAX_CLIENTE_COLA);
    sem_init(&empresa_esperando, 1, 0);
    sem_init(&cola_politico, 1, MAX_CLIENTE_COLA);
    sem_init(&politico_esperando, 1, 0);
    sem_init(&cola_entrada, 1, MAX_COLA_ENTRADA);
    sem_init(&empleado_comun, 1, 0);
    sem_init(&empleado_empresa[0], 1, 0);
    sem_init(&empleado_empresa[1], 1, 0);
    sem_init(&mutex_comun, 1, 0);
    sem_init(&mutex_empresa, 1, 0);
    sem_init(&mutex_politico, 1, 0);
    sem_init(&atencion_comun, 1, 0);
    sem_init(&atencion_empresa, 1, 0);
    sem_init(&atencion_politico, 1, 0);

    int id_empleado = 1;
    pthread_create(&tEmpleado_comun,NULL,hilo_empleado_comun,&id_empleado);

    for (int i = 0; i < EMPLEADO_EMPRESA; i++){
    	int id = i;
        pthread_create(&tEmpleado_empresa[i],NULL,hilo_empleado_empresa,&id);
        sleep(1);
    }

    for (int i = 0; i < CANT_CLIENTES; i++){
        int tipo = rand() % 3;

        switch (tipo){
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
	
    pthread_join(tEmpleado_comun,NULL);
    
    for (int i = 0; i < EMPLEADO_EMPRESA; i++){
        pthread_join(tEmpleado_empresa[i],NULL);
    }
    
	for (int i = 0; i < CANT_CLIENTES; i++){
        pthread_join(clientes[i], NULL);
    }

    sem_destroy(&cola_entrada);
    sem_destroy(&cola_cliente_comun);
    sem_destroy(&cola_empresa);
    sem_destroy(&cola_politico);
    sem_destroy(&comun_esperando);
    sem_destroy(&empresa_esperando);
    sem_destroy(&politico_esperando);
    sem_destroy(&empleado_comun);
    sem_destroy(&empleado_empresa[0]);
    sem_destroy(&empleado_empresa[1]);
    sem_destroy(&mutex_comun);
    sem_destroy(&mutex_empresa);
    sem_destroy(&mutex_politico);
    sem_destroy(&atencion_comun);
    sem_destroy(&atencion_empresa);
    sem_destroy(&atencion_politico);

    return EXIT_SUCCESS;
}

