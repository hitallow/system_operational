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
    printf("\np> Cheguei na função produtora!");
    //do
   // {
        printf("\np> produtor_f()");
        printf("\np> produzindo um item");
        int a = rand()%100;
        printf("\np> vou bloquear o mutex");
        wait_d(mutex);
        wait_d(empty);
            printf("\np> estou adcionado um valor na lista");
            buffer[empty%N] = a; 
        signal_d(full);
        signal_d(mutex);
        

   // } while (produtorI--);
    return 0 ;
}
int consumidor_f(){
 printf("\n\t\tc> Cheguei na função consumidora!");
 //do
 //{
     printf("\n\t\tc> consumidor_f()");
     wait_d(full);
     printf("\n\t\tc> vou bloquear o mutex");
     wait_d(mutex);
        printf("\n\t\tc> Estou removendo\n\n");
        printf("\n\t\tc> consumi %d ",buffer[(empty-1)%N]);
    signal_d(mutex);
    signal_d(empty);
        printf("\n\t\tc> Estou consumindo\n\n");
 //} while (consumidorI--);
    return 0 ;
}

// treades
pthread_t t_produtor[5];
pthread_t t_consumidor[5];

int main()
{
    srand(time(NULL));
    IniciarValores();
    for(int i = 0; i < 5; i++)
    {
        pthread_create(&t_produtor[i] , NULL,  (void*) produtor_f , NULL);
        pthread_create(&t_consumidor[i], NULL, (void*) consumidor_f , NULL);
    }
    for(int i = 0; i < 5; i++)
    {
        pthread_join(t_consumidor[i] , NULL);
        pthread_join(t_produtor[i] , NULL);
    }
    


    sem_delete(mutex);
    sem_delete(full);
    sem_delete(empty);
    return 0;
}

// inicia uma struct com os valores
void IniciarValores()
{    
    mutex = sem_create_d(KEY,1);
    empty = sem_create_d(KEY2,N);
    full = sem_create_d(KEY3,0); 
}