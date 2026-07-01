/*
== EJEMPLO DE PRODUCTOR-CONSUMIDOR CON SEMÁFOROS Y MUTEX EN C ==

Un productor–consumidor simple con pthreads y semáforos suele usar un buffer acotado,
un mutex y dos semáforos: uno para contar espacios libres y otro para contar ítems disponibles

IDEA DEL DISEÑO:

Buffer circular de tamaño fijo BUFFER_SIZE.
Semáforo vacios inicializado en BUFFER_SIZE (espacios libres) y llenos inicializado en 0 (elementos listos).
Un mutex (o semáforo binario) protege la sección crítica sobre el buffer y los índices in y out.

Ejemplo con 1 productor, 1 consumidor y un buffer pequeño para que se vea la sincronización.

== Para compilar: ==
gcc -o prodcons prodcons.c -pthread

== Para ejecutar: ==
./prodcons
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define N_ITEMS     20

int buffer[BUFFER_SIZE];
int in  = 0;
int out = 0;

sem_t vacios;   // cuenta espacios libres
sem_t llenos;   // cuenta ítems disponibles
pthread_mutex_t mutex;  // protege buffer, in, out

void *productor(void *arg) {
    for (int i = 0; i < N_ITEMS; i++) {
        int item = i;               // dato a producir

        sem_wait(&vacios);          // espera espacio libre
        pthread_mutex_lock(&mutex); // entra a sección crítica

        buffer[in] = item;
        printf("P -> produjo %d en %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&llenos);          // incrementa ítems disponibles

        usleep(100000);             // para ver mejor la salida
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    for (int i = 0; i < N_ITEMS; i++) {
        sem_wait(&llenos);          // espera ítems disponibles
        pthread_mutex_lock(&mutex); // entra a sección crítica

        int item = buffer[out];
        printf("C <- consumio %d desde %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&vacios);          // incrementa espacios libres

        usleep(200000);
    }
    pthread_exit(NULL);
}

int main(void) {
    pthread_t th_prod, th_cons;

    sem_init(&vacios, 0, BUFFER_SIZE);
    sem_init(&llenos, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&th_prod, NULL, productor, NULL);
    pthread_create(&th_cons, NULL, consumidor, NULL);

    pthread_join(th_prod, NULL);
    pthread_join(th_cons, NULL);

    sem_destroy(&vacios);
    sem_destroy(&llenos);
    pthread_mutex_destroy(&mutex);

    return 0;
}