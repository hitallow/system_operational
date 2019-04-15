#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "disjkrsta.h"
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

// semaforos
struct semaforos sem ;



int produtor()
{
    printf("Cheguei na função produtora!");
    do
    {

        printf("produzindo um item");
        int a = rand()%100;

        wait_d(sem.empty);
        wait_d(sem.mutex);
            printf("estou adcionado um valor na lista");
            sem.buffer[sem.empty%N] = a; 
        signal_d(sem.mutex);
        signal_d(sem.full);

    } while (produtorI--);
    return 0 ;
}
int consumidor(){
 do
 {
     printf("Cheguei na função consumidora!");
     wait_d(sem.full);
     wait_d(sem.mutex);
        printf("\t\tEstou removendo\n\n");
    signal_d(sem.mutex);
    signal_d(sem.empty);
        printf("\t\tEstou consumindo\n\n");
 } while (consumidorI--);
    return 0 ;
}

// treades
pthread_t t_produtor;
pthread_t t_consumidor;

int main()
{
    srand(time(NULL));
    printf("iniciei o main\n");
    IniciarValores();
    printf("guardei os valores no sem\n\n");
    pthread_create(&t_produtor , NULL,  (void*)produtor , NULL);
    pthread_create(&t_consumidor, NULL, (void*)consumidor , NULL);
    pthread_join(t_consumidor , NULL);
    pthread_join(t_produtor , NULL);
    printf("finalizei!\n\n");
    return 0;
}

// inicia uma struct com os valores
void IniciarValores()
{    
    sem.mutex = sem_create(KEY,1);
    sem.empty = sem_create(KEY2,N);
    sem.full = sem_create(KEY3,0);
}