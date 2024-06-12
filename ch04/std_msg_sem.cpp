#include <format>
#include <iostream>
#include <memory>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

std::size_t thread_count;
std::vector<std::string> messages;
std::vector<std::unique_ptr<std::binary_semaphore>> semaphores;

void send_msg(std::size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);

    auto thread_handles = std::vector<std::thread>{};
    thread_handles.reserve(thread_count);
    messages.resize(thread_count);
    semaphores.reserve(thread_count);
    for (size_t thread = 0; thread < thread_count; ++thread)
        semaphores.emplace_back(std::make_unique<std::binary_semaphore>(0));

    for (size_t thread = 0; thread < thread_count; ++thread)
        thread_handles.emplace_back(send_msg, thread);

    for (size_t thread = 0; thread < thread_count; ++thread)
        thread_handles[thread].join();
}

void send_msg(std::size_t rank) {
    auto dest = (rank + 1) % thread_count;

    auto my_msg = std::format("Hello to {} from {}", dest, rank);
    messages[dest] = my_msg;
    semaphores[dest]->release();

    semaphores[rank]->acquire();
    std::cout << std::format("Thread {} > {}\n", rank, messages[rank]);
}
