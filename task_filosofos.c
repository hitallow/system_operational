#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "dijkstra.h"

#define N 5
#define KEY 452
pthread_t filoso[5];
int chopstick[N];
char nomesFilosofos[5][100];

void inicializaNomes(void){
    strcpy(nomesFilosofos[0], "Platão");
    strcpy(nomesFilosofos[1], "Arquimedes");
    strcpy(nomesFilosofos[2], "Maquiavel");
    strcpy(nomesFilosofos[3], "Pitagóras");
    strcpy(nomesFilosofos[4], "NIETZSCHE");
}
void inicializaSem(void){
    int id = KEY; 
    for(int i = 0; i < 5; i++)
    {
        chopstick[i] = sem_create_d( 100*id++, 1);
    }
    
}
void* filoso_f(int i){
    do{
        wait_d(chopstick[i]);
        wait_d(chopstick[(i+1)%N]);
        printf("Sou o filósofo %s e estou comendo!",nomesFilosofos[i] );
        signal_d(chopstick[(i+1)%N]);
        signal_d(chopstick[i]);
        printf("Sou o filósofo %d e estou pensando!",nomesFilosofos[i]);
    }while(1);
    

}

void fat(int n){
    double value = 1;
    for(int i =1 ;i <=n; i++){
        value = value * i;
    }
    printf("value> %.lf",value);
}
int main(int argc, char const *argv[])
{
    for(int i = 0 ;i< 5;i++ ){
        pthread_create();
    }
    return 0;
}
