#include <mpi.h>
#include <format>
#include <iostream>
#include <string>

int const max_string = 100;

int main() {
    int comm_sz;    // number of processes
    int my_rank;    // my process rank

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank != 0) {
        auto greeting = std::format("Greetings from process {} of {}!", my_rank, comm_sz);
        MPI_Send(greeting.c_str(), static_cast<int>(greeting.size()) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        std::cout << std::format("Greetings from process {} of {}!\n", my_rank, comm_sz);
        for (int q = 1; q < comm_sz; ++q) {
            char greeting[max_string];
            MPI_Recv(greeting, max_string, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << std::format("{}\n", greeting);
        }
    }

    MPI_Finalize();
}
