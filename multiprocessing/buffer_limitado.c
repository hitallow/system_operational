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
#define MAX 1000
#define LEN_THREADS 10


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
    
        
        printf("\np> produzindo um item");
        
        printf("\np> vou entrar na seção crítica");
        wait_d(mutex);
        wait_d(empty);
            
        
            buffer[empty%N] = rand()%MAX; 
            
        signal_d(full);
        signal_d(mutex);
        

    return 0 ;
}
int consumidor_f(){
 printf("\n\t\tc> Cheguei na função consumidora!",getpid());
 //do
 //{
     printf("\n\t\tc> consumidor_f()");
     wait_d(full);
     printf("\n\t\tc> vou entrar na seção crítica");
     wait_d(mutex);
        printf("\n\t\tc> Estou removendo\n\n");
        printf("\n\t\tc> consumi %d ",buffer[(empty)%N]);
    signal_d(mutex);
    signal_d(empty);
        printf("\n\t\tc> Estou consumindo\n\n");
 //} while (consumidorI--);
    return 0 ;
}

// treades
pthread_t t_produtor[LEN_THREADS];
pthread_t t_consumidor[LEN_THREADS];

int main()
{
    srand(time(NULL));
    IniciarValores();
    for(int i = 0; i < LEN_THREADS; i++)
    {
        pthread_create(&t_produtor[i] , NULL,  (void*) produtor_f , NULL);
        pthread_create(&t_consumidor[i], NULL, (void*) consumidor_f , NULL);
    }
    for(int i = 0; i < LEN_THREADS; i++)
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