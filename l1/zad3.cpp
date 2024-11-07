#include <iostream>
#include <stdexcept>
#include <vector>
#include <initializer_list>

namespace cpplab {
    template <class T>
    class vector {
    public:
        using value_type = T;  

        T* ptr = nullptr;
        int size_val = 0;
        int capacity = 0;

        vector() {}
        ~vector() {
            delete[] ptr;
        }

        // Dodawanie elementów z tablicy
        void add(T* arr, int size_) {
            if (capacity == 0) { 
                capacity = size_; 
                ptr = new T[capacity]; 
            }

            while (size_val + size_ > capacity) {
                resize();
            }

            for (int i = 0; i < size_; i++) {
                ptr[size_val + i] = arr[i];
            }

            size_val += size_;  
        }

        // Dodawanie pojedynczego elementu
        void add(T elem) {
            if (capacity == 0) {
                capacity = 1;
                ptr = new T[capacity];
            }

            if (size_val == capacity) {
                resize();
            }

            ptr[size_val] = elem;  
            size_val++;
        }

        // Dodawanie elementów z listy inicjalizacyjnej
        void add(std::initializer_list<T> init_list) {
            if (capacity == 0) {
                capacity = init_list.size();
                ptr = new T[capacity];
            }

            while (size_val + init_list.size() > capacity) {
                resize();
            }

            for (auto elem : init_list) {
                ptr[size_val] = elem;
                size_val++;
            }
        }

        // Usuwanie elementu na podstawie indeksu
        void remove_at(std::size_t idx) {
            if (idx >= size_val) {
                throw std::out_of_range("Indeks poza zakresem");
            }

            for (std::size_t i = idx; i < size_val - 1; i++) {
                ptr[i] = ptr[i + 1];  
            }

            size_val--;  
        }

        // Usuwanie ostatniego elementu
        void pop_back() {
            if (size_val == 0) {
                throw std::out_of_range("Wektor jest pusty");
            }
            size_val--;  
        }

        int size() const{
            return size_val;
        }

        // Pobieranie elementów wektora
        const T& operator[](std::size_t idx) const {
            if (idx >= size_val) {
                throw std::out_of_range("Indeks poza zakresem");
            }
            return ptr[idx];
        }

        // Modyfikacja elementów wektora
        T& operator[](std::size_t idx) {
            if (idx >= size_val) {
                throw std::out_of_range("Indeks poza zakresem");
            }
            return ptr[idx];
        }


        void resize() {
            capacity *= 2;
            T* new_ptr = new T[capacity];
            for (int i = 0; i < size_val; ++i) {
                new_ptr[i] = ptr[i];
            }
            delete[] ptr;
            ptr = new_ptr;
        }

    };

    template <typename Vec1, typename Vec2>
    //decltype 
    auto operator*(Vec1& v1, Vec2& v2) -> decltype(v1[0] * v2[0]) {

        std::size_t size1 = v1.size();
        std::size_t size2 = v2.size();

        if (size1 != size2) {
            throw std::invalid_argument("Wektory muszą mieć taki sam rozmiar");
        }

        decltype(v1[0] * v2[0]) result = 0;  
        
        for (int i = 0; i < size1; ++i) {
            result += v1[i] * v2[i]; //obliczanie
        }

        return result; 
    }
}

int main() {
    //inicjalizacja
    cpplab::vector<int> vec1;
    vec1.add({1,2, 3});
    int arr[2] = {2,5};
    vec1.add(arr,2);
    vec1.add(8);
    cpplab::vector<char> vec11;
    vec11.add({'a','b','c','d','e','f'});

    std::vector<int> vec2 = {4, 5, 6,7,3,2};
    std::vector<float> vec3 = {1.1,1.1,1.1,1.1,1.1,1.1}; 

    //elementy 
    std::cout << "elementy wektora:" ;
    for(int i = 0; i < vec1.size(); i++){
        std::cout << '\t'<< vec1[i] ;
    }
    std::cout<<'\n';

    //mnożenie
    std::cout << "Iloczyn skalarny: " << vec2*vec1 << std::endl;  
    std::cout << "Iloczyn skalarny: " << vec1*vec2 << std::endl; 

    return 0;
}
