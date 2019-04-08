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
void reader(int sem[2] ){
    printf("\nCheguei na função leitor\n");
    do{
        // paro o mutex
        P(sem[1]);
        readcount++;
        if(readcount == 1 ){
            P(sem[0]);
        }
        // libero o mutex
        V(sem[1]);
        printf("\t\tEstou lendo!\n");
        

        // paro o mutex
        P(sem[1]);
        readcount -- ;
        if(readcount == 0)
            V(sem[0]);
        V(sem[1]);
    }while(leitorI--);  
}

int main()
{
    printf("Estou no main\n");
    
    int semaforos[2] ;
    semaforos[0] = sem_create(KEY,1);
    semaforos[1]= sem_create(KEY,1);


    // semaforos -> 1 > semaforos
    // semafotos > 2 > mutex 
    
    pthread_create(&escritor , NULL ,(void*) writer , semaforos );
    pthread_create(&leitor ,   NULL ,(void*) reader , semaforos );
    pthread_join(leitor , NULL);
    pthread_join(escritor , NULL);
    printf("\nTerminei de ler\n");

    return 0;
}
