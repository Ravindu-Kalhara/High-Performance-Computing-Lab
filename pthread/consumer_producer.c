#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_ITER 10

void* producer(void *arg);
void* consumer(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_t producer_thread, consumer_thread;
int buffer[BUFFER_SIZE];
int count = 0;

int main(){
    time_t t;
    srand((unsigned) time(&t));
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;
}

void* producer(void *arg){
    int item;

    for (int i=0; i<NUM_ITER; i++){
        item = rand();

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE){
            pthread_cond_wait(&empty, &mutex);
        }

        // access to the element in the buffer at the index count, and then increments the count variable by 1
        buffer[count++] = item;
        printf("Produced item %d \n", item);
        pthread_cond_signal(&full);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumer(void *arg){
    int item;

    for (int i=0; i<NUM_ITER; i++){
        pthread_mutex_lock(&mutex);

        while (count == 0){
            pthread_cond_wait(&full, &mutex);
        }

        item = buffer[count--];
        printf("Consumed item %d \n", item);
        pthread_cond_signal(&empty);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}