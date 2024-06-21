#include <iostream>
#include <print>
#include <mdspan>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

size_t thread_count;
size_t m, n;
std::vector<double> a;
std::vector<double> x;
std::vector<double> y;

void read_matrix(std::string_view prompt, std::mdspan<double, std::dextents<std::size_t, 2>> mat);
void read_vector(std::string_view prompt, std::span<double> vec);
void print_matrix(std::string_view title, std::mdspan<double const, std::dextents<std::size_t, 2>>);
void print_vector(std::string_view title, std::span<double const> vec);
void mat_vect(size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);

    std::println("Enter m and n");
    std::cin >> m >> n;

    a.resize(m * n);
    x.resize(n);
    y.resize(m);

    read_matrix("Enter the matrix", std::mdspan(a.data(), m, n));
    print_matrix("We read", std::mdspan(a.data(), m, n));

    read_vector("Enter the vector", x);
    print_vector("We read", x);

    auto threads = std::vector<std::thread>{};
    threads.reserve(thread_count);
    for (std::size_t thread = 0; thread < thread_count; ++thread)
        threads.emplace_back(mat_vect, thread);
    for (auto& thread : threads)
        thread.join();

    print_vector("The product is", y);
}

void read_matrix(std::string_view prompt, std::mdspan<double, std::dextents<std::size_t, 2>> mat) {
    std::println("{}", prompt);
    auto const rows = mat.extent(0);
    auto const cols = mat.extent(1);
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            std::cin >> mat[std::array{i, j}];
}

void read_vector(std::string_view prompt, std::span<double> vec) {
    std::println("{}", prompt);
    auto const s = vec.size();
    for (std::size_t i = 0; i < s; ++i)
        std::cin >> vec[i];
}

void mat_vect(size_t rank) {
    auto const local_m = m / thread_count;
    auto const my_first_row = rank * local_m;
    auto const my_last_row = (rank + 1) * local_m - 1;

    for (std::size_t i = my_first_row; i <= my_last_row; ++i) {
        y[i] = 0.0;
        for (std::size_t j = 0; j < n; ++j)
            y[i] += a[i * n + j] * x[j];
    }
}

void print_matrix(std::string_view title, std::mdspan<double const, std::dextents<std::size_t, 2>> mat) {
    std::println("{}", title);
    auto const rows = mat.extent(0);
    auto const cols = mat.extent(1);

    for (size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j)
            std::print("{:4.1f} ", mat[std::array{i, j}]);
        std::println("");
    }
}

void print_vector(std::string_view title, std::span<double const> vec) {
    std::println("{}", title);
    auto const s = vec.size();
    for (std::size_t i = 0; i < s; ++i)
        std::print("{:4.1f} ", vec[i]);
    std::println("");
}
