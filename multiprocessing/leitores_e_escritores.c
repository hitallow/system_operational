#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "dijkstra.h"

// chaves 
#define KEY2 333
#define KEY 222

pthread_t leitor[5];
pthread_t escritor[5];
int readcount = 0;


int teste = 0;
struct semaforos {
    int mutex; 
    int sem;
};
struct semaforos sem ; 
void init(){
    sem.sem  = sem_create_d(KEY,1); // sem
    sem.mutex = sem_create_d(KEY2 ,1);  //mutex
}

int writer(){
    int escritorI = 10;
    //do{
        printf("\n\twriter()\n\t w>Vou entrar na seção crítica");
        wait_d(sem.sem);
          //  sleep(3);
            printf("\n\t w> testou na seção crítica");
            printf("\n\t w> Estou escrevendo no arquivo");
        teste += 5;
        printf("\n\t w> Vou liberar o semaforo\n\n");
        

        signal_d(sem.sem);

    //} while (escritorI--);
    return 0;
}
int reader(){
    int leitorI = 10;
    //do{
        printf("\n\t\treader()");
        // bloqueio o mutex
        printf("\n\n\t\tr> Vou bloquear o mutex");
        wait_d(sem.mutex);
           // sleep(2);
            //printf("\n\t\tEstou na função reader e sou o processo %d",getpid());
            readcount ++;
            // o primeiro processo, para o semaforo
            if(readcount == 1 )
            {
                printf("\n\t\tr> Sou o primeiro processo, vou bloquear o semaforo");
                wait_d(sem.sem);
                printf("\n\n\t\tr> bloqueei o processo!");
            }else{
                printf("\n\t\tr> Já existia outro leitor, não preciso bloquear o semaforo");
            }
            // libero o mutex
        
        signal_d(sem.mutex);
        printf("\n\t\tr> Liberei o mutex, agora vou ler em paz");

        sleep(10);
    
        // bloqueio o mutex
        wait_d(sem.mutex);
            teste-=5;
            readcount-- ;
            if(readcount == 0)
            {
                printf("\n\n\t\tr> sou o último a sair, vou liberar o semafóro");
                signal_d(sem.sem);
            }else{
                printf("\n\n\t\tr> ainda existem outros leitores, vou sair e deixar o semaforo bloqueado");
            }
        signal_d(sem.mutex);
        //printf("\t\tteminei a iteração %d",10 - leitorI);
  //  }while(leitorI--);  
    return 0;
}


int main()
{
    init();
    for(int i = 0; i < 5; i++)
    {
        pthread_create(&escritor[i] , NULL ,(void*) writer , NULL );    
        pthread_create(&leitor[i] ,   NULL ,(void*) reader , NULL );
    }
    for(int i = 0; i <= 4; i++)
    {
        pthread_join(leitor[i] , NULL);
        pthread_join(escritor[i] , NULL);
    }
    
    printf("\nTerminei todos os processos\n");
    
    sem_delete(sem.mutex);
    sem_delete(sem.sem);
    printf("teste> %d",teste);
    return 0;
}
