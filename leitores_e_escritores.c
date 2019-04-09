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

struct semaforos {
    int mutex; 
    int sem;
};
struct semaforos sem ; 
void init(){
    sem.sem  = sem_create(KEY,1); // sem
    sem.mutex = sem_create(KEY,1);  //mutex
}
void writer(){
    do{
        wait_d(sem.sem);
            printf("\n\nEstou na função writter e sou o processo %d\n",getpid());
            printf("Estou escrevendo no arquivo\n");
        signal_d(sem.sem);
        printf("Liberei o semaforo\n\n");
    } while (escritorI--);
}
void reader(){
    printf("\n\n\t\tCheguei na função leitor\n");
    do{
        // bloqueio o mutex
        wait_d(sem.mutex);
            printf("\t\tEstou na função reader e sou o processo %d\n",getpid());
            readcount ++;
            // o primeiro processo, para o semaforo
            if(readcount == 1 )
            {
                printf("\t\tSou o primeiro processo, vou bloquear o sem\n");
                wait_d(sem.sem);
                printf("\t\tbloqueei o processo!\n\n");
            }
            // libero o mutex
        printf("\t\tvou liberar o mutex\n\n");
        signal_d(sem.mutex);
        printf("\t\tLiberei o mutex, agora vou ler em paz\n");
        printf("\t\tEstou lendo!\n");
        

        // bloqueio o mutex
        wait_d(sem.mutex);
            readcount -= 1 ;
            if(readcount == 0)
            {
                signal_d(sem.sem);
            }
        signal_d(sem.mutex);
    }while(leitorI--);  
}


int main()
{
    
    
    
    init();
    
    printf("mutex . %d\n\n",sem.mutex);
    printf("sem . %d\n\n",sem.sem);
    
    pthread_create(&escritor , NULL ,(void*) writer , NULL );
    pthread_create(&leitor ,   NULL ,(void*) reader , NULL );
    pthread_join(leitor , NULL);
    pthread_join(escritor , NULL);
    printf("\nTerminei de ler\n");

    return 0;
}
