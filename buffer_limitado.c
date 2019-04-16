#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "dijkstra.h"
#define N 5
#define KEY  7901
#define KEY2 1249
#define KEY3 2018


//funções utilizadas

// inicia uma struct com os valores
void IniciarValores();

// variaveis de interação
int produtorI = 20;
int consumidorI = 20;

// para facilitar o entendimento do código
struct semaforos
{
    int mutex;
    int full;
    int empty;
    int buffer[N];
};
int mutex, full, empty, buffer[N];
// semaforos
struct semaforos sem ;



int produtor_f()
{
    printf("\nCheguei na função produtora!");
    do
    {
        printf("\nprodutor_f()");
        printf("\nproduzindo um item");
        int a = rand()%100;
        wait_d(mutex);
        wait_d(empty);
            printf("\nestou adcionado um valor na lista");
            buffer[empty%N] = a; 
        signal_d(full);
        signal_d(mutex);
        

    } while (produtorI--);
    return 0 ;
}
int consumidor_f(){
 printf("\n\t\tCheguei na função consumidora!");
 do
 {
     printf("\n\t\tconsumidor_f()");
     wait_d(full);
     wait_d(mutex);
        printf("\n\t\tEstou removendo\n\n");
        printf("\n\t\tconsumi %d ",buffer[(empty-1)%N]);
    signal_d(mutex);
    signal_d(empty);
        printf("\n\t\tEstou consumindo\n\n");
 } while (consumidorI--);
    return 0 ;
}

// treades
pthread_t t_produtor;
pthread_t t_consumidor;

int main()
{
    srand(time(NULL));
    IniciarValores();
    pthread_create(&t_produtor , NULL,  (void*) produtor_f , NULL);
    pthread_create(&t_consumidor, NULL, (void*) consumidor_f , NULL);
    pthread_join(t_consumidor , NULL);
    pthread_join(t_produtor , NULL);
    sem_delete(sem.mutex);
    sem_delete(sem.full);
    sem_delete(sem.empty);
    return 0;
}

// inicia uma struct com os valores
void IniciarValores()
{    
    mutex = sem_create_d(KEY,1);
    empty = sem_create_d(KEY2,N);
    full = sem_create_d(KEY3,0); 
}