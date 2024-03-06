#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4

int arr[] = {23, 45, 67, 12, 89, 34, 56, 78, 90, 11, 45, 67, 89, 12, 34, 56, 78, 90, 11,
23, 67, 89, 12, 34, 56, 78, 90, 11, 23, 45, 89, 12, 34, 56, 78, 90, 11, 23, 45,
67, 12, 34, 56, 78, 90, 11, 23, 45, 67, 89, 34, 56, 78, 90, 11, 23, 45, 67, 89,
12, 56, 78, 90, 11, 23, 45, 67, 89, 12, 34, 78, 90, 11, 23, 45, 67, 89, 12, 34,
56, 90, 11, 23, 45, 67, 89, 12, 34, 56, 78, 11, 23, 45, 67, 89, 12, 34, 56, 78,
90};
int sum = 0;

int main(){
    int N = sizeof(arr)/sizeof(int);

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i=0; i<N; i++){
        #pragma omp atomic
        sum += arr[i];
    }

    printf("Sum = %d \n", sum);

    return 0;
}