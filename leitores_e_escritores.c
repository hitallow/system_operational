#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "disjkrsta.h"

#define KEY 123

pthread_t leitor ;
pthread_t escritor;
int readcount = 0;
int leitorI = 10;
int escritorI = 10;

struct semaforos {
    int mutex; 
    int sem;
};
void writer(struct semaforos *sem){
    do{
        wait_d(sem->sem);
            printf("Estou escrevendo no arquivo\n");
        signal_d(sem->sem);
        printf("Liberei o semaforo\n\n");
    } while (escritorI--);
}
void reader(struct semaforos *sem ){
    printf("\t\t\nCheguei na função leitor\n");
    do{
        // bloqueio o mutex
        wait_d(sem->mutex);
            readcount ++;
            // o primeiro processo, para o semaforo
            if(readcount == 1 )
            {
                printf("\t\tSou o primeiro processo, vou bloquear o sem\n");
                wait_d(sem->sem);
                printf("\t\tbloqueei o processo!\n\n");
            }
            // libero o mutex
        printf("\t\tvou liberar o mutex\n\n");
        signal_d(sem->mutex);
        printf("\t\tEstou lendo!\n");
        

        // bloqueio o mutex
        wait_d(sem->mutex);
            readcount -= 1 ;
            if(readcount == 0)
            {
                signal_d(sem->sem);
            }
        signal_d(sem->mutex);
    }while(leitorI--);  
}

int main()
{
    struct semaforos sem ; 
    
    sem.sem  = sem_create(1234,1); // sem
    sem.mutex = sem_create(1234,1);  //mutex


    // semaforos -> 1 > semaforos
    // semafotos > 2 > mutex 
    
    pthread_create(&escritor , NULL ,(void*) writer , &sem );
    pthread_create(&leitor ,   NULL ,(void*) reader , &sem );
    pthread_join(leitor , NULL);
    pthread_join(escritor , NULL);
    printf("\nTerminei de ler\n");

    return 0;
}
