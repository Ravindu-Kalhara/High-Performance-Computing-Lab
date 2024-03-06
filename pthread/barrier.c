#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 6
#define BARRIER_NUM 3

void* thread_function(void *arg);

pthread_t threads[NUM_THREADS];
pthread_barrier_t barrier;
int thread_id[NUM_THREADS];

int main(){
    pthread_barrier_init(&barrier, NULL, BARRIER_NUM);

    for (int i=0; i<NUM_THREADS; i++){
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_id[i]);
    }
    for (int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}

void* thread_function(void *arg){
    int *id = (int *) arg;
    printf("Thread %d started. \n", *id);
    pthread_barrier_wait(&barrier);
    printf("Thread %d after barrier. \n", *id);

    return NULL;
}
