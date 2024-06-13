#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t thread_count;
size_t m, n;
double* a;
double* x;
double* y;

void read_matrix(char* prompt, double a[], size_t m, size_t n);
void read_vector(char* prompt, double x[], size_t n);
void print_matrix(char* title, double a[], size_t m, size_t n);
void print_vector(char* title, double y[], size_t m);
void* pth_mat_vect(void* rank);

int main(int argc, char* argv[]) {
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    printf("Enter m and n\n");
    scanf_s("%zu%zu", &m, &n);

    a = malloc(m * n * sizeof(double));
    x = malloc(n * sizeof(double));
    y = malloc(m * sizeof(double));

    read_matrix("Enter the matrix", a, m, n);
    print_matrix("We read", a, m, n);

    read_vector("Enter the vector", x, n);
    print_vector("We read", x, n);

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_create(&thread_handles[thread], NULL, pth_mat_vect, (void*)thread);

    for (size_t thread = 0; thread < thread_count; ++thread)
        pthread_join(thread_handles[thread], NULL);

    print_vector("The product is", y, m);

    free(thread_handles);
    free(a);
    free(x);
    free(y);
}

void read_matrix(char* prompt, double a[], size_t m, size_t n) {
    printf("%s\n", prompt);
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            scanf_s("%lf", &a[i * n + j]);
}

void read_vector(char* prompt, double x[], size_t n) {
    printf("%s\n", prompt);
    for (size_t i = 0; i < n; ++i)
        scanf_s("%lf", &x[i]);
}

void* pth_mat_vect(void* rank) {
    size_t my_rank = (size_t)rank;
    size_t local_m = m / thread_count;
    size_t my_first_row = my_rank * local_m;
    size_t my_last_row = (my_rank + 1) * local_m - 1;

    for (size_t i = my_first_row; i <= my_last_row; ++i) {
        y[i] = 0.0;
        for (size_t j = 0; j < n; ++j)
            y[i] += a[i * n + j] * x[j];
    }

    return NULL;
}

void print_matrix(char* title, double a[], size_t m, size_t n) {
    printf("%s\n", title);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j)
            printf("%4.1f ", a[i * n + j]);
        printf("\n");
    }
}

void print_vector(char* title, double y[], size_t m) {
    printf("%s\n", title);
    for (size_t i = 0; i < m; ++i)
        printf("%4.1f ", y[i]);
    printf("\n");
}
