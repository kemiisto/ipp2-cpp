#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

size_t thread_count;
size_t n;
double sum;
pthread_mutex_t mutex;

void* thread_sum(void* rank);

void get_args(int argc, char* argv[]);

double serial_pi(size_t n);

int main(int argc, char* argv[]) {
    get_args(argc, argv);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    sum = 0.0;

    for (size_t thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, thread_sum, (void*)thread);
        pthread_join(thread_handles[thread], NULL);
    }

    sum = 4.0 * sum;
    printf("With n = %lld terms,\n", n);
    printf("   Our estimate of pi = %.15f\n", sum);
    sum = serial_pi(n);
    printf("   Single thread est  = %.15f\n", sum);
    printf("                   pi = %.15f\n", 4.0 * atan(1.0));

    free(thread_handles);
    pthread_mutex_destroy(&mutex);
}

void* thread_sum(void* rank) {
    size_t my_rank = (size_t)rank;
    size_t my_n = n / thread_count;
    size_t my_first_i = my_n * my_rank;
    size_t my_last_i = my_first_i + my_n;

    double my_sum = 0.0;
    int factor = (my_first_i % 2 == 0) ? 1 : -1;
    for (size_t i = my_first_i; i < my_last_i; ++i, factor = -factor)
        my_sum += (double)factor / (2 * (double)i + 1);

    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

double serial_pi(size_t n) {
    double sum = 0.0;
    int factor = 1;
    for (size_t i = 0; i < n; ++i, factor = -factor)
        sum += (double)factor / (2 * (double)i + 1);
    return 4.0 * sum;

}

void get_args(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    n = strtoll(argv[2], NULL, 10);
}
