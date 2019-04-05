#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include "dijkstra.h"


#define KEY 123

int main()
{
    int sem ; 
    sem = sem_create(KEY ,1);
    printf("Um semáforo foi criado com o o indentificador %d\n",sem);
    if(fork() == 0){
        printf("\tProcesso filho usa o recurso\n");
        P(sem);
        sleep(8);
        printf("\tProcesso filho libera o recurso\n");
        V(sem);
        sleep(1);

    }else{
        sleep(1);
        printf("Processo PAI bloqueia ao tentar acessar recurso\n");
        P(sem);
        printf("recurso disponivel para o processo PAI \n");
        sem_delete(sem);
    }
    
    
    return 0;
}
