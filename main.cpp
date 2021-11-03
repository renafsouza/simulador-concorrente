/*
 *  Simulador de Atendimento Bancário na Pandemia
 *  Autor: Renã Ferreira de Souza
 *  Data: 3/nov/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>
void print();
void *printLoop (void *arg);
void *clientBehavior (void *ptr);


// PARÂMETROS
float tick = 0.2; // Quantos segundos em um tick
int n_seats = 9; // Numero de assentos
int n_columns = 3; // Numero de colunas de assentos (não interfere com a simulação)
int client_interval[2] = {1,14}; // Limite inferior e superior do intervalo de segundos entre a chegada de novos clientes
int atendimento_interval[2] = {5,12}; // Limite inferior e superior da duração de atendimento


pthread_mutex_t lock;
pthread_mutex_t seatsLock;

int esperandoCount = 0;
int atendidosCount = 0;
int goneCount = 0;
int aguardandoGerente = 0;
int atendendo = 0;


int *seats;

int tempoDeAtendimento = 0;


int main (int argc, char *argv[])
{
    if(argc==8){
        // Pega parâmetros da linha de comando
        tick = std::__cxx11::stof(argv[1]);
        n_seats = std::__cxx11::stoi(argv[2]);
        n_columns = std::__cxx11::stoi(argv[3]);
        client_interval[0] = std::__cxx11::stoi(argv[4]);
        client_interval[1] = std::__cxx11::stoi(argv[5]);
        atendimento_interval[0] = std::__cxx11::stoi(argv[6]);
        atendimento_interval[1] = std::__cxx11::stoi(argv[7]);
    }

    seats = (int*) malloc (n_seats*sizeof(int));
    if (seats==NULL) exit (1);
    for(int i =0;i<n_seats;i++){
        seats[i] = 0;
    }


    pthread_t clients[n_seats];
    pthread_t printLoopThread;
    pthread_create (&printLoopThread, NULL, printLoop, NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(1){
        pthread_create (&clients[esperandoCount], NULL, clientBehavior, NULL);
        int delay = rand()%client_interval[1]+client_interval[0];
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(delay*1000*tick)));
        
    }
    exit (0);
}

void *clientBehavior (void *arg){
    int availableSeat = -1;
    pthread_mutex_lock(&seatsLock);
    for(int i=0;i<n_seats;i++){
        if(!seats[i]){
            availableSeat=i;
            seats[i] = 1;
            break;
        }
    }
    pthread_mutex_unlock(&seatsLock);

    if(availableSeat!=-1){
        esperandoCount++;
        // printf("tempo de atendimento: %d\n",tempoDeAtendimento);

        pthread_mutex_lock(&lock);
            tempoDeAtendimento = rand()%atendimento_interval[1]+atendimento_interval[0];
            esperandoCount--;
            aguardandoGerente=1;
            pthread_mutex_lock(&seatsLock);
            seats[availableSeat] = 0;
            pthread_mutex_unlock(&seatsLock);
            while(--tempoDeAtendimento){
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000*tick)));
            }
        atendendo=0;
        atendidosCount++;
        pthread_mutex_unlock(&lock);

    }else{
        goneCount++;
    }
}

void print(){
    // Imprime cabeçalho
    printf("G = Gerente\nC = Cliente\n- = Assento disponível\n\n");
    // Imprime parâmetros e variáveis
    printf("Tick: %.2fs\n",tick);
    printf("Numero de assentos: %d\n",n_seats);
    printf("Clientes atendidos: %d\n",atendidosCount);
    printf("Clientes esperando: %d\n",esperandoCount);
    printf("Clientes que foram embora: %d\n",goneCount);
    printf("Tempo restante de atendimento (%ds-%ds): %ds\n",atendimento_interval[0],atendimento_interval[1],tempoDeAtendimento);
    printf("Intervalo de novos clientes: %ds-%ds\n",client_interval[0],client_interval[1]);
    printf("\n");

    // Imprime bancada de atendimento
    if(atendendo){
        printf("G | C");
    }else if(aguardandoGerente){
        aguardandoGerente=0;
        atendendo = 1;
        printf("- | C");
    }else{
        printf("- | -");
    }
    printf("\n\n\n");

    // Imprime assentos
    for(int i=0;i<=(n_seats/n_columns);i++){
        for(int j=0;j<n_columns&&(i*n_columns+j)<n_seats;j++){
            if(seats[i*n_columns+j]){
                // imprime assento ocupado
                printf("C  ");
            }else{
                // imprime assento disponível
                printf("-  ");
            }
        }
        // Imprime espaço suficiente para não haver contágio entre clientes
        printf("\n\n");
    }
}

void *printLoop (void *arg){
    while(1){
        system ("clear");
        print();
        // garante que não vai atualizar mais do que 60 vezes por segundo
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}