#include <iostream>
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

void read_matrix(std::string_view prompt, std::mdspan<double, std::dextents<std::size_t, 2>> a);
void read_vector(std::string_view prompt, std::span<double> x);
void print_matrix(std::string_view title, std::mdspan<double const, std::dextents<std::size_t, 2>>);
void print_vector(std::string_view title, std::span<double const> y);
void pth_mat_vect(size_t rank);

int main(int argc, char* argv[]) {
    thread_count = std::stol(argv[1]);

    std::cout << "Enter m and n\n";
    std::cin >> m >> n;

    a.resize(m * n);
    x.resize(n);
    y.resize(m);

    read_matrix("Enter the matrix", std::mdspan(a.data(), m, n));
    print_matrix("We read", std::mdspan(a.data(), m, n));

    read_vector("Enter the vector", x);
    print_vector("We read", x);

    auto thread_handles = std::vector<std::thread>{};
    thread_handles.reserve(thread_count);
    for (std::size_t thread = 0; thread < thread_count; ++thread)
        thread_handles.emplace_back(pth_mat_vect, thread);
    for (auto& thread : thread_handles)
        thread.join();

    print_vector("The product is", y);
}

void read_matrix(std::string_view prompt, std::mdspan<double, std::dextents<std::size_t, 2>> a) {
    std::cout << prompt << '\n';
    auto const m = a.extent(0);
    auto const n = a.extent(1);
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            std::cin >> a[std::array{i, j}];
}

void read_vector(std::string_view prompt, std::span<double> x) {
    std::cout << prompt << '\n';
    auto const n = x.size();
    for (size_t i = 0; i < n; ++i)
        std::cin >> x[i];
}

void pth_mat_vect(size_t rank) {
    size_t local_m = m / thread_count;
    size_t my_first_row = rank * local_m;
    size_t my_last_row = (rank + 1) * local_m - 1;

    for (size_t i = my_first_row; i <= my_last_row; ++i) {
        y[i] = 0.0;
        for (size_t j = 0; j < n; ++j)
            y[i] += a[i * n + j] * x[j];
    }
}

void print_matrix(std::string_view title, std::mdspan<double const, std::dextents<std::size_t, 2>> a) {
    std::cout << title << '\n';
    auto const m = a.extent(0);
    auto const n = a.extent(1);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j)
            printf("%4.1f ", a[std::array{i, j}]);
        printf("\n");
    }
}

void print_vector(std::string_view title, std::span<double const> y) {
    std::cout << title << '\n';
    auto const m = y.size();
    for (size_t i = 0; i < m; ++i)
        printf("%4.1f ", y[i]);
    printf("\n");
}
