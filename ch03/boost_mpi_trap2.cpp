#include <mpi.h>
#include <format>
#include <iostream>

// Get the input values.
void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);

// Calculate local integral.
double trap(double left_endpt, double right_endpt, int trap_count, double base_len);

// Function we're integrating.
double f(double x);

int main() {
    int my_rank, comm_sz, n = 1024;
    double a = 0.0, b = 3.0;
    double total_int;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    get_input(my_rank, comm_sz, &a, &b, &n);

    auto h = (b - a) / n;            // h is the same for all processes
    auto local_n = n / comm_sz;      // so is the number of trapezoids

    auto local_a = a + my_rank * local_n * h;
    auto local_b = local_a + local_n * h;
    auto local_int = trap(local_a, local_b, local_n, h);

    // Add up the integrals calculated by each process.
    if (my_rank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_int = local_int;
        for (int source = 1; source < comm_sz; ++source) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }

    // Print the result.
    if (my_rank == 0) {
        std::cout << std::format("With n = {} trapezoids, our estimate\n", n);
        std::cout << std::format("of the integral from {} to {} = {}\n", a, b, total_int);
    }

    MPI_Finalize();
}

void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
    if (my_rank == 0) {
        std::cout << "Enter a, b, and n\n";
        std::cin >> *a_p >> *b_p >> *n_p;
        for (int dest = 1; dest < comm_sz; ++dest) {
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
    auto estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
    for (int i = 1; i <= trap_count - 1; ++i) {
        auto x = left_endpt + i * base_len;
        estimate += f(x);
    }
    return estimate * base_len;
}

double f(double x) { return x * x; }