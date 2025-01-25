#include <iostream>
#include <concepts>
#include <vector>
#include <string> 


template <typename T>
concept Check = requires(T a, T b) {
    { a * b } -> std::convertible_to<T>;
};

template <typename Vec1, typename Vec2>
requires Check<typename Vec1::value_type> && Check<typename Vec2::value_type>
auto operator*(const Vec1& v1, const Vec2& v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Wektory muszą mieć taki sam rozmiar");
    }

    using ValueType = decltype(v1[0] * v2[0]);
    ValueType result = 0;

    for (std::size_t i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }

    return result;
}

int main() {
    std::vector<double> v1 = {1.1, 2.3, 3.2};
    std::vector<int> v2 = {4, 5, 6};
    std::vector<std::string> v3 = {"bajo", "jajo", "bajojajo"};
    std::vector<std::string> v4 = {"bajo", "jajo", "bajojajo"};

    try {
        auto result = v2 * v1; 
        std::cout << "Iloczyn skalarny: " << result << std::endl;

        // Próba mnożenia wektorów stringów (błąd kompilacji)
        // auto result2 = v3 * v4;
        // std::cout << "Iloczyn skalarny: " << result2 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
