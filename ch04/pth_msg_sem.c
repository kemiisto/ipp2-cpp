#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

const int MSG_MAX = 100;

size_t thread_count;
char** messages;
sem_t* semaphores;

void* send_msg(void* rank);

int main(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    messages = malloc(thread_count * sizeof(char*));
    semaphores = malloc(thread_count * sizeof(sem_t));
    for (size_t thread = 0; thread < thread_count; ++thread) {
        messages[thread] = NULL;
        // initialize all semaphores to 0 i.e. locked
        sem_init(&semaphores[thread], 0, 0);
    }

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL, send_msg, (void*)thread);

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_join(thread_handles[thread], NULL);

    for (size_t thread = 0; thread < thread_count; ++thread) {
        free(messages[thread]);
        sem_destroy(&semaphores[thread]);
    }
    free(semaphores);
    free(messages);
    free(thread_handles);
}

void* send_msg(void* rank) {
    size_t my_rank = (size_t)rank;
    size_t dest = (my_rank + 1) % thread_count;
    char* my_msg = malloc(MSG_MAX * sizeof(char));

    sprintf(my_msg, "Hello to %zu from %zu", dest, my_rank);
    messages[dest] = my_msg;
    sem_post(&semaphores[dest]);    // unlock the semaphore of dest

    sem_wait(&semaphores[my_rank]); // wait for our semaphore to be unlocked
    printf("Thread %zu > %s\n", my_rank, messages[my_rank]);

    return NULL;
}
