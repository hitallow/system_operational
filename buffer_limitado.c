#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "disjkrsta.h"
#define N 5


//funções utilizadas

// inicia uma struct com os valores
void IniciarValores(struct semaforos *sem);

// variaveis de interação
int produtorI = 20;
int consumidorI = 20;

// para facilitar o entendimento do código
struct semaforos
{
    int mutex;
    int full;
    int empty;
} ;

// semaforos
struct semaforos sem ;




// treades
pthread_t t_produtor, t_consumidor;


void* produtor()
{
    do
    {
        printf("produzindo um item");
        P(sem.empty);
        P(sem.mutex);
            printf("estou adcionado um valor na lista");
        V(sem.mutex);
        V(sem.full);

    } while (produtorI--);
}
void* consumidor(){
 do
 {
     P(sem.full);
     P(sem.mutex);
        printf("\t\tEstou removendo\n\n");
    V(sem.mutex);
    V(sem.empty);
        printf("\t\tEstou consumindo\n\n");
 } while (consumidorI--);
 
}
int main(int argc, char const *argv[])
{
    printf("iniciei o main\n");
    IniciarValores(&sem);
    printf("guardei os valores no sem\n\n");
    printf("sem -> mutex > %d",sem.mutex);
    printf("sem -> full > %d",sem.full);
    printf("sem -> empty > %d",sem.empty);

    pthread_create(&t_produtor , NULL,(void*)produtor , NULL);
    pthread_create(&t_consumidor, NULL, (void*)consumidor , NULL);
    
    pthread_join(t_consumidor , NULL);
    pthread_join(t_produtor , NULL);
    printf("finalizei mermão!\n\n");
    return 0;
}

// inicia uma struct com os valores
void IniciarValores(struct semaforos *sem){
    
    sem->mutex = sem_create(1,0);
    sem->empty = sem_create(N,N);
    sem->full = sem_create(5,0);
    printf("sem -> mutex > %d\n",sem->mutex);
    printf("sem -> full > %d\n",sem->full);
    printf("sem -> empty > %d\n",sem->empty);
    
}