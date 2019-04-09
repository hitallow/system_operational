#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "disjkrsta.h"

// chaves 
#define KEY2 111
#define KEY 222

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
    sem.mutex = sem_create(KEY2 ,1);  //mutex
}

int writer(){
    do{
        printf("\ncomeçei a iteração %d",10 - escritorI);
        wait_d(sem.sem);
            printf("\n\nEstou na função writter e sou o processo %d\n",getpid());
            printf("Estou escrevendo no arquivo\n");
        signal_d(sem.sem);
        printf("Liberei o semaforo\n\n");
        printf("teminei a iteração %d\n\n",10 - escritorI );
    } while (escritorI--);
    return 0;
}
int reader(){
    printf("\n\n\t\tCheguei na função leitor\n");
    do{
        printf("\t\tcomeçei a iteração %d",10 - leitorI);
        // bloqueio o mutex
        wait_d(sem.mutex);
            printf("\n\n\t\tEstou na função reader e sou o processo %d\n",getpid());
            readcount ++;
            // o primeiro processo, para o semaforo
            if(readcount == 1 )
            {
                printf("\t\tSou o primeiro processo, vou bloquear o sem, %d\n",readcount);
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
            readcount-- ;
            if(readcount == 0)
            {
                signal_d(sem.sem);
            }
        signal_d(sem.mutex);
        printf("\t\tteminei a iteração %d",10 - leitorI);
    }while(leitorI--);  
    return 0;
}


int main()
{

    init();
    
    printf("mutex -> %d\n\n",sem.mutex);
    printf("sem -> %d\n\n"  ,sem.sem);
    
    pthread_create(&escritor , NULL ,(void*) writer , NULL );
    pthread_create(&leitor ,   NULL ,(void*) reader , NULL );
    pthread_join(leitor , NULL);
    pthread_join(escritor , NULL);
    printf("\nTerminei todos os processos\n");

    return 0;
}
