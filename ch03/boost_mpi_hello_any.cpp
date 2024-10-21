#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <print>
#include <string>

int main() {
    auto const env = boost::mpi::environment{};
    auto const comm = boost::mpi::communicator{};

    auto const comm_sz = comm.size();
    auto const my_rank = comm.rank();

    if (my_rank != 0) {
        auto const greeting = std::format("Greetings from process {} of {}!", my_rank, comm_sz);
        comm.send(0, 0, greeting);
    } else {
        std::println("Greetings from process {} of {}!", my_rank, comm_sz);
        for (int q = 1; q < comm_sz; ++q) {
            auto greeting = std::string{};
            comm.recv(boost::mpi::any_source, 0, greeting);
            std::println("{}", greeting);
        }
    }
}
