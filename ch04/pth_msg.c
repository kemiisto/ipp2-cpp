#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int MSG_MAX = 100;

size_t thread_count;
char** messages;

void* send_msg(void* rank);

int main(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    messages = malloc(thread_count * sizeof(char*));
    for (size_t thread = 0; thread < thread_count; ++thread)
        messages[thread] = NULL;

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL, send_msg, (void*)thread);

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_join(thread_handles[thread], NULL);

    for (size_t thread = 0; thread < thread_count; ++thread)
        free(messages[thread]);
    free(messages);
    free(thread_handles);
}

void* send_msg(void* rank) {
    size_t my_rank = (size_t)rank;
    size_t dest = (my_rank + 1) % thread_count;
    size_t source = (my_rank + thread_count - 1) % thread_count;
    char* my_msg = malloc(MSG_MAX * sizeof(char));

    sprintf(my_msg, "Hello to %zu from %zu", dest, my_rank);
    messages[dest] = my_msg;

    if (messages[my_rank] != NULL)
        printf("Thread %zu > %s\n", my_rank, messages[my_rank]);
    else
        printf("Thread %zu > No message from %zu\n", my_rank, source);

    return NULL;
}
