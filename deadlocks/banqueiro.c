#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "dijkstra.h"

// variaveis booleanas
#define FALSE 0
#define TRUE 1


#define MUTEX 1

// maximo limite individual de recursos
#define MAXLIMIT 15

// semaforo
int mutex;

// numero de clientes
int n;

// numero de recursos
int m;

// estruturas utilizadas pelo algoritmo

// recursos disponiveis
int * disp;
// maximo de recursos para cada processo
int ** max;
// quantidade de recursos alocadas por processo
int ** aloc;
// quantidade de recursos que cada processo ainda precisa
int ** ne;

// threads dos n clientes
pthread_t * ncli;

// funcoes utilizadas

// verifica se o estado eh seguro
int seguranca();

// verifica se uma requisicao eh valida
int requisicao(int, int *);

// a ser rodada pelas threads dos clientes
void cliente(int);

// inicializa os valores disponiveis dos recursos
void inicia_recursos();

// inicializa os valores dos clientes:
//  - valores de max[][]
//  - valores de aloc[][]
//  - valores de ne[][]
void inicia_clientes();

// verifica se um cliente ja finalizou, se tem todos os recursos necessario
// se sim, libera seus recursos alocados
int finaliza_cliente(int);

// gera uma requisicao aleatoria
int * gera_requisicao();

// gera um valor aleatorio
// de 1 a lim, se zero = 1
// de 0 a lim, se zero = 0
int gera_rand(int lim, int zero);

// imprime o estado do banco
void print_banco();

int step2b(int* trab, int index);



int main(int argc, char ** argv)
{
    if ( argc != 3 )
    {
        printf("Uso: %s num_clientes num_recursos\n", argv[0]);
        return 1;
    }

    int i;
    
    // iniciando a semente do random
    srand(time(NULL));

    // num clients
    n = atoi(argv[1]);
    
    // num resources
    m = atoi(argv[2]);

    // criando os semaforos
    mutex = sem_create(MUTEX, 1);
    
    // iniciando os recursos
    inicia_recursos();
    
    // iniciando os clientes
    inicia_clientes();

    print_banco();
    
    // excluindo os semaforos
    sem_delete(mutex);

    // liberando a memoria alocada
    free(disp);
    free(max);
    free(aloc);
    free(ne);
    
    return 0;
}

void inicia_recursos()
{
    int i;

    // gerando um vetor de recursos disponiveis
    disp = (int *)malloc(m * sizeof(int));

    for (i = 0; i < m; i++)
    {
        disp[i] = gera_rand(MAXLIMIT, 1);
    }
}

void inicia_clientes()
{
    int i, j;

    // criando as matrizes do banqueiro
    max  = (int **) malloc(n * sizeof(int *));
    aloc = (int **) malloc(n * sizeof(int *));
    ne   = (int **) malloc(n * sizeof(int *));
    
    // gerando uma lista de threads de clientes
    ncli = malloc(n * sizeof(pthread_t));

    // iniciando os valores maximos de cada cliente
    for (i = 0; i < n; i++)
    {
        max[i]  = (int *) malloc(m * sizeof(int));
        aloc[i] = (int *) malloc(m * sizeof(int));
        ne[i]   = (int *) malloc(m * sizeof(int));

        for (j = 0; j < m; j++)
        {
            max[i][j]  = gera_rand(disp[j], 1);
            aloc[i][j] = 0;
            ne[i][j]   = max[i][j];
        }
    
        // iniciando as threads dos clientes
        pthread_create(&ncli[i], NULL, (void *)cliente, (void *)i);
    }
    
    // esperando as threads
    for (i = 0; i < n; i++)
    {
        pthread_join(ncli[i], NULL);
    }
    
}

int * gera_requisicao(int cliente)
{
    static int * req;
    req = malloc(m * sizeof(int));

    int i;
    for (i = 0; i < m; i++)
    {
        req[i] = gera_rand(ne[cliente][i], 0);
    }

    return req;
}

int requisicao(int index, int * req)
{

    //n é a quantidade de clientes 
    // m é a quantidade de recursos 


    // verifica se req <= ne
    for(int i = 0 ; i < m; i ++){
        if(req[i] > ne[index][i]){
            return FALSE;
        }
    }


    // verifica se req <= disp
    for(int i = 0 ; i < m; i++){
        if(req[i] > disp[i]){
            return FALSE;
        }
    }

    // simulo a alocação
    for(int i = 0 ;i < m; i++){
        disp[i] = disp[i] - req[i];
    }

    for(int i = 0 ; i < m; i ++){
        aloc[index][i] += req[i];
    }
    for(int i = 0 ; i < m; i++){
        ne[index][i] -= req[i];
    }

    // testa se o banco esta em estado seguro
    if(seguranca())
    {
        // retorna verdadeiro se a requisicao eh valida
        
        return TRUE;
    }else
    {
        // desfaz simulacao (roll back) em caso de requisicao invalida
        for(int i = 0 ;i < m; i++)
        {
            disp[i] = disp[i] + req[i];
        }

        for(int i = 0 ; i < m; i ++)
        {
            aloc[index][i] -= req[i];
        }
        for(int i = 0 ; i < m; i++)
        {
            ne[index][i] += req[i];
        }
        // requisicao negada
        return FALSE;
    }
        

}

int seguranca()
{

    int i, j;

    int ne_menor;

    int * trab = malloc(m * sizeof(int));
    int * fim = malloc(n * sizeof(int));

    // copia disp para trab
    for( i = 0 ; i < m; i ++ ){
        trab[i] = disp[i];
    }

    // iniciando todos de fim como false
    for(i = 0; i < n; i++){
        fim[i] = FALSE;
    }
     
    int flag = n;
    i = 0;
    while(i<n){
        if(!fim[i] && step2b(trab , i)){            
            for(j = 0 ;j < m ; j++){
                trab[j] += aloc[i][j];
            }
            fim[i] = TRUE;
        }
        i++;
        if(i==n && flag){
            i = 0;
            flag -- ;
        }
    }

    for(i =0 ;i<n;i++){
        if(!(fim[i])){
            free(trab);
            free(fim);
            return FALSE;
        }
    }
    free(trab);
    free(fim);
    
    return TRUE;
}
int step2b(int* trab, int index){
    for(int i= 0 ;i < m ; i ++){
        if(!(ne[index][i] <= trab[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}

int finaliza_cliente(int i)
{
    int j;

    // verifica se cliente nao tem mais necessidades
    for (j = 0; j < m; j++)
    {
        // cliente ainda nao terminou
        if ( ne[i][j] != 0 )
        {
            return 0;
        }
    }

    // cliente terminou

    // limpa seus recursos
    for (j = 0; j < m; j++)
    {
        // devolve para os disponiveis
        disp[j] += aloc[i][j];
        aloc[i][j] = 0;
    }

    return 1;
}

void cliente(int i)
{
    int executa = 1, j;

    int * req;
    
    while(executa)
    {
        // garante acesso exclusivo ao buffer
        P(mutex);
    
        printf(": Cliente %d ganhou acesso\n",i);
        
        printf(": Cliente %d gerou a requisição: ",i);

        req = gera_requisicao(i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ",req[j]);
        }

        printf("\n");

        // enviando a requisição ao banco
        if ( requisicao(i, req) == 1 )
        {
            printf(": Cliente %d executou sua requisicao\n",i);

            // verifica se o cliente terminou
            if ( finaliza_cliente(i) == 1 )
            {
            
                printf(": Cliente %d finalizou\n",i);

                // cliente ja terminou, seus recursos foram liberados
                executa = 0;
            }
        }
        else
        {
            printf(": Cliente %d teve sua requisicao negada\n",i);
        }

        print_banco();
        
        // libera o acesso
        V(mutex);
    }
}

int gera_rand(int limit, int zero)
{
    if (zero == 1)
    {
        // 1 a limit
        return (rand() % limit) + 1;
    }
    
    // 0 a limit
    return rand() % (limit + 1);
    
}

void print_banco()
{
    int i, j;

    printf("\t== BANCO ==\n");

    printf("Recursos disponiveis\n");
    printf("\t    i: ");
    for (j = 0; j < m; j++)
    {
        printf(" %2d ",j);
    }
    
    printf("\n\t       ");
    for (j = 0; j < m; j++)
    {
        printf(" %2d ",disp[j]);
    }

    printf("\n");

    printf("MAX dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ",i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ",max[i][j]);
        }
    
        printf(" ]\n");
    }
    
    printf("Aloc dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ",i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ",aloc[i][j]);
        }
    
        printf(" ]\n");
    }
    
    printf("Ne dos clientes\n");
    for (i = 0; i < n; i++)
    {
        printf("\ti: %d [ ",i);

        for (j = 0; j < m; j++)
        {
            printf(" %2d ",ne[i][j]);
        }
    
        printf(" ]\n");
    }
    
    printf("========================\n");
    printf("\n");
}
