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


void writer(int *sem){

    do{
        P(sem[0]);
            printf("Estou escrevendo no arquivo\n");
        V(sem[0]);
    } while (escritorI--);
}
void reader(int* sem ){
    printf("\t\t\nCheguei na função leitor\n");
    do{
        // bloqueio o mutex
        P(sem[1]);
            readcount ++;
            // o primeiro processo, para o semaforo
            if(readcount == 1 ){
                printf("\t\tSou o primeiro processo, vou bloquear o sem\n");
                //P(sem[0]);
            }
            // libero o mutex
        printf("\t\tvou liberar o mutex\n\n");
        V(sem[1]);
        printf("\t\tEstou lendo!\n");
        

        // bloqueio o mutex
        P(sem[1]);
            readcount -= 1 ;
            if(readcount == 0)
            {
                V(sem[0]);
            }
        V(sem[1]);
    }while(leitorI--);  
}

int main()
{
    printf("Estou no main\n");
    
    int semaforos[2] ;
    semaforos[0] = sem_create(2,1); // sem
    semaforos[1]= sem_create(KEY,1);  //mutex


    // semaforos -> 1 > semaforos
    // semafotos > 2 > mutex 
    
    pthread_create(&escritor , NULL ,(void*) writer , semaforos );
    pthread_create(&leitor ,   NULL ,(void*) reader , semaforos );
    pthread_join(leitor , NULL);
    pthread_join(escritor , NULL);
    printf("\nTerminei de ler\n");

    return 0;
}
