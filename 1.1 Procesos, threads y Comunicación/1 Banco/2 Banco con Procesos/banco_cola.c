#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/wait.h>

#define CANT_CLIENTES 80
#define MAX_COLA_ENTRADA 30
#define MAX_CLIENTE_COLA 15
#define EMPLEADO_CLIENTES 1
#define EMPLEADO_EMPRESA 2

#define COMUN 0
#define EMPRESA 1
#define POLITICO 2

//Tipos de mensajes
#define TIPO_CLIENTE_COMUN 101
#define TIPO_CLIENTE_EMPRESA 102
#define TIPO_CLIENTE_POLITICO 103
#define TIPO_ENTRADA 104

#define TIPO_EMPLEADO_COMUN 301
#define TIPO_EMPLEADO_EMPRESA 302

#define TIPO_COMUN_ESPERANDO 401
#define TIPO_EMPRESA_ESPERANDO 402
#define TIPO_POLITICO_ESPERANDO 403

#define TIPO_TERMINA_COMUN 501
#define TIPO_TERMINA_EMPRESA 502
#define TIPO_TERMINA_POLITICO 503

#define TIPO_ATENCION_COMUN 601
#define TIPO_ATENCION_EMPRESA 602
#define TIPO_ATENCION_POLITICO 603

//Claves 
#define KEY_ENTRADA 200
#define KEY_CLIENTE_COMUN 201
#define KEY_CLIENTE_EMPRESA 202
#define KEY_CLIENTE_POLITICO 203
#define KEY_EMPLEADO_COMUN 204
#define KEY_EMPLEADO_EMPRESA 205
#define KEY_COMUN_ESPERANDO 206
#define KEY_EMPRESA_ESPERANDO 207
#define KEY_POLITICO_ESPERANDO 208
#define KEY_TERMINA_COMUN 209
#define KEY_TERMINA_EMPRESA 210
#define KEY_TERMINA_POLITICO 211
#define KEY_ATENCION_COMUN 212
#define KEY_ATENCION_EMPRESA 213
#define KEY_ATENCION_POLITICO 214
struct Mensaje
{
    long tipo;
    char message[100];
};
typedef struct Mensaje msg;

int cola_entrada, 
    cola_cliente_comun,
    cola_politico,
    cola_empresa, 
    
    cola_comun_esperando, 
    cola_politico_esperando,
    cola_empresa_esperando,
    
    cola_empleado_comun, 
    cola_empleado_empresa,
    
    cola_termina_comun,
    cola_termina_empresa,
    cola_termina_politico,
    
    cola_atencion_comun,
    cola_atencion_empresa,
    cola_atencion_politico;

    msg msg_entrada;
    msg msg_cliente_comun;
    msg msg_cliente_empresa;
    msg msg_cliente_politico;
    
    msg msg_empleado_comun;
    msg msg_empleado_empresa;
    
    msg msg_comun_esperando; 
    msg msg_politico_esperando;
    msg msg_empresa_esperando;
    
    msg msg_termina_comun;
    msg msg_termina_empresa;
    msg msg_termina_politico;  
      
    msg msg_atencion_comun;
    msg msg_atencion_empresa;
	msg msg_atencion_politico;

void cliente_comun(int id){
    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_cliente_comun = msgget((key_t)KEY_CLIENTE_COMUN, 0666);
    int cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, 0666);
    int cola_comun_esperando = msgget((key_t)KEY_COMUN_ESPERANDO, 0666);
    int cola_atencion_comun = msgget((key_t)KEY_ATENCION_COMUN, 0666);
    int cola_termina_comun = msgget((key_t)KEY_TERMINA_COMUN, 0666);

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) != -1){
        printf("ClienteComun %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sleep(2);
        msgrcv(cola_cliente_comun, &msg_cliente_comun, sizeof(msg_cliente_comun), TIPO_CLIENTE_COMUN, 0);
        printf("ClienteComun %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClienteComun %d :: Ingreso en la cola de cliente Comun\n", id);
        msgsnd(cola_comun_esperando, &msg_comun_esperando, sizeof(msg_comun_esperando), 0);

        if (msgrcv(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), TIPO_EMPLEADO_COMUN, IPC_NOWAIT) == -1)
       	{
       		msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
       	}
        else{
            msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
            msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
        }        
        printf("ClienteComun %d :: Esperando se atendido por empleado cliente Comun\n", id);
        msgrcv(cola_atencion_comun, &msg_atencion_comun, sizeof(msg_atencion_comun), TIPO_ATENCION_COMUN, 0);
        printf("ClienteComun %d :: Se retira de la cola de cliente Comun para ser atendido\n", id);
        msgrcv(cola_termina_comun, &msg_termina_comun, sizeof(msg_termina_comun), TIPO_TERMINA_COMUN,0);
        printf("ClienteComun %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else{
        printf("ClienteComun %d :: No hay lugar en la cola de entrada, y se retira del banco\n", id);
    }
}

void cliente_empresa(int id){
    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_empresa = msgget((key_t)KEY_CLIENTE_EMPRESA, 0666);
    int cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, 0666);
    int cola_empresa_esperando = msgget((key_t)KEY_EMPRESA_ESPERANDO, 0666);
    int cola_atencion_empresa = msgget((key_t)KEY_ATENCION_EMPRESA, 0666);
    int cola_termina_empresa = msgget((key_t)KEY_TERMINA_EMPRESA, 0666);

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) != -1){
        printf("ClienteEmpresa %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sleep(2);
        msgrcv(cola_empresa, &msg_cliente_empresa, sizeof(msg_cliente_empresa), TIPO_CLIENTE_EMPRESA, 0);
        printf("ClienteEmpresa %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClienteEmpresa %d :: Ingreso en la cola de cliente Empresa\n", id);
        msgsnd(cola_empresa_esperando, &msg_empresa_esperando, sizeof(msg_empresa_esperando), 0);

        if (msgrcv(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), TIPO_EMPLEADO_EMPRESA, IPC_NOWAIT) == -1)
       	{
       		msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
       	}
        else{
            msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
            msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
        }        
        
        printf("ClienteEmpresa %d :: Esperando se atendido por empleado cliente Empresa\n", id);
        msgrcv(cola_atencion_empresa, &msg_atencion_empresa, sizeof(msg_atencion_empresa), TIPO_ATENCION_EMPRESA, 0);
        printf("ClienteEmpresa %d :: Se retira de la cola de cliente Empresa para ser atendido\n", id);
        msgrcv(cola_termina_empresa, &msg_termina_empresa, sizeof(msg_termina_empresa), TIPO_TERMINA_EMPRESA,0);
        printf("ClienteEmpresa %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else{
        printf("ClienteEmpresa %d :: No hay lugar en la cola de entrada, y se retira del banco\n", id);
    }
}

void cliente_politico(int id){
    int cola_entrada = msgget((key_t)KEY_ENTRADA, 0666);
    int cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, 0666);
    int cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, 0666);
    int cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, 0666);
    int cola_politico_esperando = msgget((key_t)KEY_POLITICO_ESPERANDO, 0666);
    int cola_atencion_politico = msgget((key_t)KEY_ATENCION_POLITICO, 0666);
    int cola_termina_politico = msgget((key_t)KEY_TERMINA_POLITICO, 0666);

    if (msgrcv(cola_entrada, &msg_entrada, sizeof(msg_entrada), TIPO_ENTRADA, IPC_NOWAIT) != -1){
        printf("ClientePolitico %d :: Hay lugar en la mesa de entrada y espera en la misma.\n", id);
        sleep(2);
        msgrcv(cola_politico, &msg_cliente_politico, sizeof(msg_cliente_politico), TIPO_CLIENTE_POLITICO, 0);
        printf("ClientePolitico %d :: Se retira de la cola de entrada\n", id);
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
        printf("ClientePolitico %d :: Ingreso en la cola de cliente Politico\n", id);
        msgsnd(cola_politico_esperando, &msg_politico_esperando, sizeof(msg_politico_esperando), 0);
        	
        if (msgrcv(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), TIPO_EMPLEADO_COMUN, IPC_NOWAIT) == -1)
       	{
       		msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
       	}
        else{
            msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
            msgsnd(cola_empleado_comun, &msg_empleado_comun, sizeof(msg_empleado_comun), 0);
        }   
       	if (msgrcv(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), TIPO_EMPLEADO_EMPRESA, IPC_NOWAIT) == -1)
       	{
       		msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
       		msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
        }
        else{
            msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
       		msgsnd(cola_empleado_empresa, &msg_empleado_empresa, sizeof(msg_empleado_empresa), 0);
        }        
        printf("ClientePolitico %d :: Esperando se atendido por empleado del banco\n", id);
        msgrcv(cola_atencion_politico, &msg_atencion_politico, sizeof(msg_atencion_politico), TIPO_ATENCION_POLITICO, 0);
        printf("ClientePolitico %d :: Se retira de la cola de cliente Politico para ser atendido\n", id);
        msgrcv(cola_termina_politico, &msg_termina_politico, sizeof(msg_termina_politico), TIPO_TERMINA_POLITICO,0);
        printf("ClientePoitico %d :: Se retira del banco\n", id);
        fflush(NULL);
    }
    else{
        printf("ClientePolitico %d :: No hay lugar en la cola de entrada, y se retira del banco\n", id);
    }
}

void empleado_comun(int id){
	int cola_cliente_comun = msgget((key_t)KEY_CLIENTE_COMUN, 0666);
    int cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, 0666);
    int cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, 0666);
    int cola_politico_esperando = msgget((key_t)KEY_POLITICO_ESPERANDO, 0666);
    int cola_comun_esperando = msgget((key_t)KEY_COMUN_ESPERANDO, 0666);
    int cola_atencion_politico = msgget((key_t)KEY_ATENCION_POLITICO, 0666);
    int cola_atencion_comun = msgget((key_t)KEY_ATENCION_COMUN, 0666);
    int cola_termina_politico = msgget((key_t)KEY_TERMINA_POLITICO, 0666);
    int cola_termina_comun = msgget((key_t)KEY_TERMINA_COMUN, 0666);
    int no_atendi = 0;

    msgrcv(cola_empleado_comun,&msg_empleado_comun,sizeof(msg_empleado_comun),TIPO_EMPLEADO_COMUN,0);
    
	while(no_atendi < 3){
		printf("\nEmpleadoComun %d :: Revisa si hay clientes politicos.\n", id);
		if(msgrcv(cola_politico_esperando,&msg_politico_esperando, sizeof(msg_politico_esperando),TIPO_POLITICO_ESPERANDO,IPC_NOWAIT) != -1){
            printf("EmpleadoComun %d :: Estoy atendiendo un cliente politico.\n\n", id);
            msgsnd(cola_atencion_politico, &msg_atencion_politico, sizeof(msg_atencion_politico),0);
            msgsnd(cola_politico, &msg_cliente_politico, sizeof(msg_cliente_politico),0);
            sleep(5);
            msgsnd(cola_termina_politico, &msg_termina_politico, sizeof(msg_termina_politico),0);
        }
        else{
            printf("EmpleadoComun %d :: Revisa si hay clientes comun.\n", id);
            
            if(msgrcv(cola_comun_esperando,&msg_comun_esperando, sizeof(msg_comun_esperando),TIPO_COMUN_ESPERANDO,IPC_NOWAIT) != -1){
            	printf("EmpleadoComun %d :: Estoy atendiendo un cliente comun.\n\n", id);
            	msgsnd(cola_atencion_comun, &msg_atencion_comun, sizeof(msg_atencion_comun),0);
                msgsnd(cola_cliente_comun, &msg_cliente_comun, sizeof(msg_cliente_comun),0);
            	sleep(5);
            	msgsnd(cola_termina_comun, &msg_termina_comun, sizeof(msg_termina_comun),0);
        }
            else{
            	printf("EmpleadoComun %d :: No hay clientes por atender.\n\n", id);
                msgrcv(cola_empleado_comun,&msg_empleado_comun,sizeof(msg_empleado_comun),TIPO_EMPLEADO_COMUN,0);
                no_atendi++;
            }
        }
	}
}

void empleado_empresa(int id){
	int cola_empresa = msgget((key_t)KEY_CLIENTE_EMPRESA, 0666);
    int cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, 0666);
    int cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, 0666);
    int cola_politico_esperando = msgget((key_t)KEY_POLITICO_ESPERANDO, 0666);
    int cola_empresa_esperando = msgget((key_t)KEY_EMPRESA_ESPERANDO, 0666);
    int cola_atencion_politico = msgget((key_t)KEY_ATENCION_POLITICO, 0666);
    int cola_atencion_empresa = msgget((key_t)KEY_ATENCION_EMPRESA, 0666);
    int cola_termina_politico = msgget((key_t)KEY_TERMINA_POLITICO, 0666);
    int cola_termina_empresa = msgget((key_t)KEY_TERMINA_EMPRESA, 0666);
    int no_atendi = 0;

    msgrcv(cola_empleado_empresa,&msg_empleado_empresa, sizeof(msg_empleado_empresa), TIPO_EMPLEADO_EMPRESA, 0);
    
	while(no_atendi < 3){
		printf("\nEmpleadoEmpresa %d :: Revisa si hay clientes politicos.\n", id);
		
		if(msgrcv(cola_politico_esperando,&msg_politico_esperando, sizeof(msg_politico_esperando),TIPO_POLITICO_ESPERANDO,IPC_NOWAIT) != -1){
            printf("EmpleadoEmpresa %d :: Estoy atendiendo un cliente politico.\n\n", id);
            msgsnd(cola_atencion_politico, &msg_atencion_politico, sizeof(msg_atencion_politico),0);
            msgsnd(cola_politico, &msg_cliente_politico, sizeof(msg_cliente_politico),0);
            sleep(5);
            msgsnd(cola_termina_politico, &msg_termina_politico, sizeof(msg_termina_politico),0);
        }
        else{
            printf("EmpleadoEmpresa %d :: Revisa si hay clientes empresa.\n", id);
            
            if(msgrcv(cola_empresa_esperando,&msg_empresa_esperando, sizeof(msg_empresa_esperando),TIPO_EMPRESA_ESPERANDO,IPC_NOWAIT) != -1){
            	printf("EmpleadoEmpresa %d :: Estoy atendiendo un cliente empresa.\n\n", id);
            	msgsnd(cola_atencion_empresa, &msg_atencion_empresa, sizeof(msg_atencion_empresa),0);
                msgsnd(cola_empresa, &msg_cliente_empresa, sizeof(msg_cliente_empresa),0);
            	sleep(5);
            	msgsnd(cola_termina_empresa, &msg_termina_empresa, sizeof(msg_termina_empresa),0);
            }
            else{
            	printf("EmpleadoEmpresa %d :: No hay clientes por atender.\n\n", id);
                msgrcv(cola_empleado_empresa,&msg_empleado_empresa, sizeof(msg_empleado_empresa), TIPO_EMPLEADO_EMPRESA, 0);
            	no_atendi++;
            }
        }
	}
}

int main(){

    srand(time(NULL));

    cola_entrada = msgget((key_t)KEY_ENTRADA, IPC_CREAT | 0666);
    cola_cliente_comun = msgget((key_t)KEY_CLIENTE_COMUN, IPC_CREAT | 0666);
    cola_empresa = msgget((key_t)KEY_CLIENTE_EMPRESA, IPC_CREAT | 0666);
    cola_politico = msgget((key_t)KEY_CLIENTE_POLITICO, IPC_CREAT | 0666);
    
    cola_empleado_comun = msgget((key_t)KEY_EMPLEADO_COMUN, IPC_CREAT | 0666);
    cola_empleado_empresa = msgget((key_t)KEY_EMPLEADO_EMPRESA, IPC_CREAT | 0666);
    
    cola_comun_esperando =  msgget((key_t)KEY_COMUN_ESPERANDO, IPC_CREAT | 0666);
    cola_politico_esperando = msgget((key_t)KEY_POLITICO_ESPERANDO, IPC_CREAT | 0666);
    cola_empresa_esperando = msgget((key_t)KEY_EMPRESA_ESPERANDO, IPC_CREAT | 0666); 
    
    cola_termina_comun = msgget((key_t)KEY_TERMINA_COMUN, IPC_CREAT | 0666);
    cola_termina_empresa = msgget((key_t)KEY_TERMINA_EMPRESA, IPC_CREAT | 0666);
    cola_termina_politico = msgget((key_t)KEY_TERMINA_POLITICO, IPC_CREAT | 0666);
    
    cola_atencion_comun = msgget((key_t)KEY_ATENCION_COMUN, IPC_CREAT | 0666);
    cola_atencion_empresa = msgget((key_t)KEY_ATENCION_EMPRESA, IPC_CREAT | 0666);
    cola_atencion_politico = msgget((key_t)KEY_ATENCION_POLITICO, IPC_CREAT | 0666);
    
    msg_entrada.tipo = TIPO_ENTRADA;
    msg_cliente_comun.tipo = TIPO_CLIENTE_COMUN;
    msg_cliente_empresa.tipo = TIPO_CLIENTE_EMPRESA;
    msg_cliente_politico.tipo = TIPO_CLIENTE_POLITICO;
    
    msg_empleado_comun.tipo = TIPO_EMPLEADO_COMUN;
    msg_empleado_empresa.tipo = TIPO_EMPLEADO_EMPRESA;
    
    msg_comun_esperando.tipo = TIPO_COMUN_ESPERANDO;
    msg_politico_esperando.tipo = TIPO_POLITICO_ESPERANDO;
    msg_empresa_esperando.tipo = TIPO_EMPRESA_ESPERANDO;
    
    msg_termina_comun.tipo = TIPO_TERMINA_COMUN;
    msg_termina_empresa.tipo = TIPO_TERMINA_EMPRESA;
    msg_termina_politico.tipo = TIPO_TERMINA_POLITICO;
    
    msg_atencion_comun.tipo = TIPO_ATENCION_COMUN;
    msg_atencion_empresa.tipo = TIPO_ATENCION_EMPRESA;
	msg_atencion_politico.tipo = TIPO_ATENCION_POLITICO;
    
    if (fork() == 0){
    	empleado_comun(0);
        exit(0);
    }

    for (int i = 0; i < EMPLEADO_EMPRESA; i++){
	 	if (fork() == 0){
        	empleado_empresa(i);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_COLA_ENTRADA; i++){
        msgsnd(cola_entrada, &msg_entrada, sizeof(msg_entrada), 0);
    }

    for (int i = 0; i < MAX_CLIENTE_COLA; i++){
        msgsnd(cola_cliente_comun, &msg_cliente_comun, sizeof(msg_cliente_comun), 0);
        msgsnd(cola_empresa, &msg_cliente_empresa, sizeof(msg_cliente_empresa), 0);
        msgsnd(cola_politico, &msg_cliente_politico, sizeof(msg_cliente_politico), 0);
    }

    for (int i = 0; i < CANT_CLIENTES; i++){
        int tipo = rand() % 3;

        switch (tipo){
		    case COMUN:
		        if (fork() == 0){
		            cliente_comun(i);
		            exit(0);
		        }
		        break;
		    case EMPRESA:
		        if (fork() == 0){
		            cliente_empresa(i);
		            exit(0);
		        }
		        break;
		    case POLITICO:
		        if (fork() == 0){
		            cliente_politico(i);
		            exit(0);
		        }
		        break;
		    }
    }
    
    for(int i = 0; i < (EMPLEADO_CLIENTES + EMPLEADO_EMPRESA); i++){
    	wait(NULL);
    }

    for (int i = 0; i < CANT_CLIENTES; i++){
        wait(NULL);
    }

    msgctl(cola_entrada, IPC_RMID, NULL);
    msgctl(cola_cliente_comun, IPC_RMID, NULL);
    msgctl(cola_empresa, IPC_RMID, NULL);
    msgctl(cola_politico, IPC_RMID, NULL);
    msgctl(cola_empleado_comun, IPC_RMID, NULL);
    msgctl(cola_empleado_empresa, IPC_RMID, NULL);
    msgctl(cola_comun_esperando, IPC_RMID, NULL);
    msgctl(cola_politico_esperando, IPC_RMID, NULL);
	msgctl(cola_empresa_esperando, IPC_RMID, NULL);
	msgctl(cola_termina_comun, IPC_RMID, NULL);
    msgctl(cola_termina_empresa, IPC_RMID, NULL);
    msgctl(cola_termina_politico, IPC_RMID, NULL);
    msgctl(cola_atencion_comun, IPC_RMID, NULL);
    msgctl(cola_atencion_empresa, IPC_RMID, NULL);
	msgctl(cola_atencion_politico, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}
