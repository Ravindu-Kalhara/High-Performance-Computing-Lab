#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define STEP_SIZE 2
#define NUM_STEPS 100
#define DIMENTION 100   // walking area = DIMENTION x DIMENTION

void* update_position(void *arg);
void* read_position(void *args);

struct Random_Walker{
    int x;
    int y;
};
struct Random_Walker walker;
pthread_rwlock_t rwlock;
pthread_t update_thread;
pthread_t read_thread;

int main(){
    time_t t;
    srand((unsigned) time(&t));

    // set initial position
    walker.x = DIMENTION/2;
    walker.y = DIMENTION/2;

    pthread_rwlock_init(&rwlock, NULL);
    pthread_create(&update_thread, NULL, update_position, &walker);
    pthread_create(&read_thread, NULL, read_position, &walker);

    pthread_join(update_thread, NULL);
    pthread_join(read_thread, NULL);
    printf("Final Position: (%d, %d) \n", walker.x, walker.y);
    pthread_rwlock_destroy(&rwlock);

    return 0;
}

void* update_position(void *arg){
    struct Random_Walker *walker = (struct Random_Walker *) arg;
    int direction;

    for (int i=0; i<NUM_STEPS; i++){
        pthread_rwlock_wrlock(&rwlock);
        direction = rand() % 4; // select direction
        switch (direction){
            // walk
            case 0:
                walker->x += STEP_SIZE;
                break;
            case 1:
                walker->y += STEP_SIZE;
                break;
            case 2:
                walker->x-= STEP_SIZE;
                break;
            case 3:
                walker->y -= STEP_SIZE;
                break;
            default:
                break;
        }
        // check the boundries
        if (walker->x > DIMENTION){
            walker->x = DIMENTION;
        }
        if (walker->x < 0){
            walker->x = 0;
        }
        if (walker->y > DIMENTION){
            walker->y = DIMENTION;
        }
        if (walker->y < 0){
            walker->y = 0;
        }
        printf("Possition (in update_position): (%d, %d) \n", walker->x, walker->y);
        sleep(0.1);
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

void* read_position(void *arg){
    struct Random_Walker *walker = (struct Random_Walker *) arg;

    for (int i=0; i<NUM_STEPS/3; i++){
        pthread_rwlock_rdlock(&rwlock);
        printf("Possition (in read_position): (%d, %d) \n", walker->x, walker->y);
        sleep(0.1);
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}