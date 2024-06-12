#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::size_t thread_count;

void hello(std::size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);
    auto thread_handles = std::vector<std::thread>{};
    thread_handles.reserve(thread_count);
    for (std::size_t thread = 0; thread < thread_count; ++thread)
        thread_handles.emplace_back(hello, thread);

    std::cout << "hello from the main thread\n";

    for (std::size_t thread = 0; thread < thread_count; ++thread)
        thread_handles[thread].join();
}

void hello(std::size_t rank) {
    std::cout << std::format("hello from thread {} of {}\n", rank, thread_count);
}
