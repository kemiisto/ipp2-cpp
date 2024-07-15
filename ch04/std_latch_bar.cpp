#include "timer.h"
#include <iostream>
#include <latch>
#include <memory>
#include <print>
#include <string>
#include <thread>
#include <vector>

#define DEBUG

constexpr auto barrier_count = 100;

std::size_t thread_count;
std::vector<std::unique_ptr<std::latch>> latches;

void thread_work(std::size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);

    auto threads = std::vector<std::thread>{};
    threads.reserve(thread_count);

    latches.reserve(thread_count);
    for (std::size_t i = 0; i < barrier_count; i++)
        latches.emplace_back(std::make_unique<std::latch>(thread_count));

    double start = get_time();
    for (std::size_t thread = 0; thread < thread_count; ++thread)
        threads.emplace_back(thread_work, thread);

    for (auto&& thread : threads)
        thread.join();

    double finish = get_time();
    std::println("Elapsed time = {:e} seconds", finish - start);
}

void thread_work(std::size_t rank) {
    for (std::size_t i = 0; i < barrier_count; ++i) {
        latches[i]->arrive_and_wait();
#ifdef DEBUG
        if (rank == 0) {
            std::println("All threads entered barrier {}", i);
            std::flush(std::cout);
        }
#endif
    }
}
