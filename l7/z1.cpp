#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <stdexcept>
#include <memory>
#include <typeinfo>

namespace cpplab {

// Zadanie 2: Własny alokator
template <typename T>
class allocator {
public:
    using value_type = T;

    allocator() noexcept = default;
    template <typename U>
    allocator(const allocator<U>&) noexcept {}


    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();
        void* ptr = ::operator new(n * sizeof(T));
        return static_cast<T*>(ptr);
    }


    void deallocate(T* p, std::size_t) noexcept {
        ::operator delete(p);
    }
};

}

// Zadanie 1: Funkcja do obliczania iloczynu skalarnego
void dot_product(const std::vector<double>& a, const std::vector<double>& b,
                 std::promise<double> result) {
    if (a.empty() || b.empty()) {
        result.set_exception(std::make_exception_ptr(std::invalid_argument("Vectors cannot be empty")));
        return;
    }
    if (a.size() != b.size()) {
        result.set_exception(std::make_exception_ptr(std::invalid_argument("Vectors must have the same size")));
        return;
    }
    double dot = std::inner_product(a.begin(), a.end(), b.begin(), 0.0);
    result.set_value(dot);
}

int main() {
    // Zadanie 1: Iloczyn skalarny
    std::cout << "=== Zadanie 1: Iloczyn skalarny ===" << std::endl;

    std::vector<std::vector<double>> vectors_a = {
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0},
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0},
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}
    };
    std::vector<std::vector<double>> vectors_b = vectors_a;

    std::vector<std::future<double>> futures;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < vectors_a.size(); ++i) {
        std::promise<double> promise;
        futures.push_back(promise.get_future());
        threads.emplace_back(dot_product, std::cref(vectors_a[i]), std::cref(vectors_b[i]), std::move(promise));
    }

    double total_sum = 0.0;
    for (auto& future : futures) {
        try {
            total_sum += future.get();
        } catch (const std::exception& ex) {
            std::cerr << "Exception: " << ex.what() << '\n';
        }
    }

    // Zamykanie wątków
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Suma wyników: " << total_sum << '\n';

        // Zadanie 2: Własny alokator
        std::cout << "\n=== Zadanie 2: Własny alokator ===" << std::endl;

                         cpplab::allocator<int> alloc;

    std::cout << "Typ alokowany: " << typeid(cpplab::allocator<int>::value_type).name() << std::endl;
    int* p_test = alloc.allocate(1);
    std::cout << "Nieinicjalizowana pamięć: " << *p_test << std::endl;
    *p_test = 3;
    std::cout << "Zapisana wartość: " << *p_test << std::endl;
    alloc.deallocate(p_test, 1);

    std::vector<int, cpplab::allocator<int>> v;
    v.push_back(13);
    v.push_back(23);

    std::cout << "Ostatni element wektora: " << *(v.end() - 1) << std::endl;

    return 0;
}
