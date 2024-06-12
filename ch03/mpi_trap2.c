#include <mpi.h>
#include <stdio.h>

// get the input values
void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);

// calculate local integral
double trap(double left_endpt, double right_endpt, int trap_count, double base_len);

// function we're integrating
double f(double x);

int main() {
    int my_rank, comm_sz, n;
    double a, b;
    double total_int;
    int source;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    get_input(my_rank, comm_sz, &a, &b, &n);

    double h = (b - a) / n;
    int local_n = n / comm_sz;

    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_int = trap(local_a, local_b, local_n, h);

    // Add up the integrals calculated by each process.
    if (my_rank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_int = local_int;
        for (source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }

    // Print the result.
    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
    }

    MPI_Finalize();
}

void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
    if (my_rank == 0) {
        printf("Enter a, b, and n\n");
        fflush(stdout);
        scanf_s("%lf %lf %d", a_p, b_p, n_p);
        for (int dest = 1; dest < comm_sz; dest++) {
            MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

double trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
    double estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
    for (int i = 1; i <= trap_count - 1; ++i) {
        double x = left_endpt + i * base_len;
        estimate += f(x);
    }
    return estimate * base_len;
}

double f(double x) {
    return x * x;
}
