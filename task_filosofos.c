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
        chopstick[i] = sem_create_d( 100+id++, 1);
    }
    
}
void* filoso_f(int i){

    //printf(">>> vou iniciar %s value i = %d\n",nomesFilosofos[i],i);
    //do{
        printf("sou o filósofo %s e vou pegar o primeiro chopstrik\n",nomesFilosofos[i]);
        wait_d(chopstick[i]);
        printf("\tsou o filósofo %s e vou pegar o segundo chopstrik\n",nomesFilosofos[i]);
        wait_d(chopstick[(i+1)%N]);
        printf("\t\tSou o filósofo %s e  agora vou começar comer!\n\n",nomesFilosofos[i] );
        sleep(0);
        printf("sou o filósofo %s e terminei de comer vou liberar o primeiro chopstrik\n",nomesFilosofos[i]);
        signal_d(chopstick[(i+1)%N]);
        printf("sou o filósofo %s e terminei de comer vou liberar o segundo chopstrik\n",nomesFilosofos[i]);
        signal_d(chopstick[i]);
        printf("\tSou o filósofo %s e estou pensando!\n\n",nomesFilosofos[i]);
   // }while(1);
    

}


int main()
{
    inicializaSem();
    inicializaNomes();
    for(int i = 0 ;i< 5;i++ ){
        //printf(">>>>>>>>>>vou criar a trad com %d\n",i);
        pthread_create(&filoso[i], NULL, (void*)  filoso_f , (i));    
    }
    for(int i = 0;i<5;i++){
        pthread_join(filoso[i],NULL);
    }
    for(int i = 0; i < 5; i++)
    {
        sem_delete(chopstick[i]);
    }
    
    
    return 0;
}
