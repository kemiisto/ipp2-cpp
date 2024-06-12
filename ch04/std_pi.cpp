#include <cmath>
#include <print>
#include <string>
#include <thread>
#include <vector>

size_t thread_count;
size_t n;
double sum;

void thread_sum(size_t rank);

void get_args(int argc, char* argv[]);

double serial_pi(size_t n);

int main(int argc, char* argv[]) {
    get_args(argc, argv);
    auto thread_handles = std::vector<std::thread>();
    thread_handles.reserve(thread_count);

    sum = 0.0;

    for (size_t thread = 0; thread < thread_count; ++thread) {
        auto& thr = thread_handles.emplace_back(thread_sum, thread);
        thr.join();
    }

    sum = 4.0 * sum;
    std::print("With n = {} terms,\n", n);
    std::print("   Our estimate of pi = {:.15f}\n", sum);
    sum = serial_pi(n);
    std::print("   Single thread est  = {:.15f}\n", sum);
    std::print("                   pi = {:.15f}\n", 4.0 * atan(1.0));
}

void thread_sum(size_t rank) {
    auto my_n = n / thread_count;
    auto my_first_i = my_n * rank;
    auto my_last_i = my_first_i + my_n;

    auto factor = (my_first_i % 2 == 0) ? 1 : -1;
    for (size_t i = my_first_i; i < my_last_i; ++i, factor = -factor)
        sum += static_cast<double>(factor) / (2 * static_cast<double>(i) + 1);
}

double serial_pi(size_t n) {
    auto sum = 0.0;
    auto factor = 1;
    for (size_t i = 0; i < n; ++i, factor = -factor)
        sum += static_cast<double>(factor) / (2 * static_cast<double>(i) + 1);
    return 4.0 * sum;

}

void get_args(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);
    n = std::stol(argv[2]);
}
