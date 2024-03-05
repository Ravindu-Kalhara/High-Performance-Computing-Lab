#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

void sum_array(int arr[], int size);
void *thread_function(void *arg);

struct Range{
    int start;
    int end;
};
pthread_t threads[NUM_THREADS];
struct Range ranges[NUM_THREADS];
int arr[] = {23, 45, 67, 12, 89, 34, 56, 78, 90, 11, 45, 67, 89, 12, 34, 56, 78, 90, 11,
23, 67, 89, 12, 34, 56, 78, 90, 11, 23, 45, 89, 12, 34, 56, 78, 90, 11, 23, 45,
67, 12, 34, 56, 78, 90, 11, 23, 45, 67, 89, 34, 56, 78, 90, 11, 23, 45, 67, 89,
12, 56, 78, 90, 11, 23, 45, 67, 89, 12, 34, 78, 90, 11, 23, 45, 67, 89, 12, 34,
56, 90, 11, 23, 45, 67, 89, 12, 34, 56, 78, 11, 23, 45, 67, 89, 12, 34, 56, 78,
90};
pthread_mutex_t mutex;
int sum = 0;

int main(){
    int N = sizeof(arr)/sizeof(int);
    sum_array(arr, N);

    int segment = N/NUM_THREADS;
    for (int i=0; i<NUM_THREADS; i++){
        ranges[i].start = i*segment;
        ranges[i].end = (i+1)*segment - 1;
    }
    if (N > segment*NUM_THREADS){
        ranges[NUM_THREADS-1].end = N-1;
    }

    for (int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_function, &ranges[i]);
    }
    for (int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("Sum (parallel) = %d \n", sum);

    return 0;
}

void sum_array(int arr[], int N) {
    int sum = 0;
    for (int i=0; i<N; i++) {
        sum += arr[i];
    }
    printf("Sum (serial) = %d \n", sum);
}

void *thread_function(void *arg){
    struct Range *range = (struct Range *) arg;
    int local_sum = 0;
    // the arrow operator (->) is used to access the members of a structure (or a union) through a pointer to that structure. 
    // on the other hand, the dot operator (.) is used to access the members of a structure directly.
    for (int i=range->start; i<=range->end; i++){
        local_sum += arr[i];
    }
    pthread_mutex_lock(&mutex);
    sum += local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}