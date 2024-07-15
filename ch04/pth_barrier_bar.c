#include "timer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#define BARRIER_COUNT 100

size_t thread_count;
pthread_barrier_t barriers[BARRIER_COUNT];

void* thread_work(void* rank);

int main(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));
    for (size_t i = 0; i < BARRIER_COUNT; i++)
        pthread_barrier_init(&barriers[i], NULL, thread_count);

    double const start = get_time();
    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_create(&thread_handles[thread], NULL, thread_work, (void*)thread);

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_join(thread_handles[thread], NULL);

    double const finish = get_time();
    printf("Elapsed time = %e seconds\n", finish - start);

    for (size_t i = 0; i < BARRIER_COUNT; i++)
        pthread_barrier_destroy(&barriers[i]);
    free(thread_handles);
    return 0;
}

void* thread_work(void* rank) {
#ifdef DEBUG
    size_t my_rank = (size_t)rank;
#endif
    for (size_t i = 0; i < BARRIER_COUNT; ++i) {
        pthread_barrier_wait(&barriers[i]);
#ifdef DEBUG
        if (my_rank == 0) {
            printf("All threads entered barrier %llu\n", i);
            fflush(stdout);
        }
#endif
    }
    return NULL;
}
