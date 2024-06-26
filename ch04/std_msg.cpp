#include <format>
#include <print>
#include <string>
#include <thread>
#include <vector>

std::size_t thread_count;
std::vector<std::string> messages;

void send_msg(std::size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);

    auto thread_handles = std::vector<std::thread>{};
    thread_handles.reserve(thread_count);
    messages.resize(thread_count);

    for (size_t thread = 0; thread < thread_count; ++thread)
        thread_handles.emplace_back(send_msg, thread);

    for (auto& thread : thread_handles)
        thread.join();
}

void send_msg(std::size_t rank) {
    auto dest = (rank + 1) % thread_count;
    auto source = (rank + thread_count - 1) % thread_count;

    auto my_msg = std::format("Hello to {} from {}", dest, rank);
    messages[dest] = my_msg;

    if (!messages[rank].empty()) std::println("Thread {} > {}", rank, messages[rank]);
    else std::println("Thread {} > No message from {}", rank, source);
}
