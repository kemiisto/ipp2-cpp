#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

size_t thread_count;

void* hello(void* rank);

int main(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_create(&thread_handles[thread], NULL, hello, (void*)thread);

    printf("hello from the main thread\n");

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
}

void* hello(void* rank) {
    size_t my_rank = (size_t)rank;
    printf("hello from thread %zu of %zu\n", my_rank, thread_count);
    return NULL;
}
