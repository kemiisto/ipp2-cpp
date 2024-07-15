#include <print>
#include <string>
#include <thread>
#include <vector>

std::size_t thread_count;

void hello(std::size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);
    auto threads = std::vector<std::thread>{};
    threads.reserve(thread_count);
    for (std::size_t thread = 0; thread < thread_count; ++thread)
        threads.emplace_back(hello, thread);

    std::print("hello from the main thread\n");

    for (auto&& thread : threads)
        thread.join();
}

void hello(std::size_t rank) {
    std::print("hello from thread {} of {}\n", rank, thread_count);
}
