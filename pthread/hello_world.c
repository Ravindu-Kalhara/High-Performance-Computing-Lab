#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];
void *thread_function(void *arg);

int main(){
    for (int i=0; i<NUM_THREADS; i++){
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }
    for (int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void *thread_function(void *arg){
    // "arg" is being cast to a pointer to an integer (int *) to interpret arg as a pointer to an integer.
    int *id_ptr = (int *)arg;
    printf("Thread %d says hello world! \n", *id_ptr);
    return NULL;
}
