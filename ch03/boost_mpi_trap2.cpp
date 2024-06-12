#include <boost/mpi.hpp>
#include <format>
#include <iostream>

// get the input values
void get_input(const boost::mpi::communicator& comm, double& a, double& b, int& n);

// calculate local integral
double trap(double left_endpt, double right_endpt, int trap_count, double base_len);

// function we're integrating
double f(double x);

int main() {
    int n {};
    double a {}, b {};
    double total_int {};

    auto const env = boost::mpi::environment{};
    auto const comm = boost::mpi::communicator{};

    auto const my_rank = comm.rank();
    auto const comm_sz = comm.size();

    get_input(comm, a, b, n);

    auto const h = (b - a) / n;
    auto const local_n = n / comm_sz;

    auto const local_a = a + my_rank * local_n * h;
    auto const local_b = local_a + local_n * h;
    auto local_int = trap(local_a, local_b, local_n, h);

    // Add up the integrals calculated by each process.
    if (my_rank != 0) {
        comm.send(0, 0, local_int);
    } else {
        total_int = local_int;
        for (int source = 1; source < comm_sz; ++source) {
            comm.recv(source, 0, local_int);
            total_int += local_int;
        }
    }

    // Print the result.
    if (my_rank == 0) {
        std::cout << std::format("With n = {} trapezoids, our estimate\n", n);
        std::cout << std::format("of the integral from {} to {} = {}\n", a, b, total_int);
    }
}

void get_input(const boost::mpi::communicator& comm, double& a, double& b, int& n) {
    auto const my_rank = comm.rank();
    auto const comm_sz = comm.size();

    if (my_rank == 0) {
        std::cout << "Enter a, b, and n\n";
        std::cin >> a >> b >> n;
        for (int dest = 1; dest < comm_sz; ++dest) {
            comm.send(dest, 0, a);
            comm.send(dest, 0, b);
            comm.send(dest, 0, n);
        }
    } else {
        comm.recv(0, 0, a);
        comm.recv(0, 0, b);
        comm.recv(0, 0, n);
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

double f(double x) {
    return x * x;
}
