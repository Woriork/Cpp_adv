#include <iostream>
#include <initializer_list>
#include <string>
#include <utility>

/*wielka trójka: 
                -konstruktor kopiujący 
                -operator przypisania (Kopiuje zawartość z I elem do II)
                -destruktor
 wielka piątka:
                +konstruktor przenoszący(przejmuje zasoby)
                +przenoszący operator przypisania(przenosi zasoby z I do II)
*/

/*memcpy:
               funkcja do kopiowania bloków pamięci. 
                    -kopiowania obiektów typów niebędących POD nie jest zalecane, 
                     ponieważ zdefiniowane przez użytkownika konstruktory 
                     i operatory przypisania nie zostanę wywołane.
*/

namespace cpplab {
    template <class T>
    class vector {
        T* ptr = nullptr;
        int size_val = 0;
        int capacity = 0;
        
    public:
        vector() = default;
        
        // Konstruktor kopiujący
        vector(const vector<T>& other)
            : size_val(other.size_val), capacity(other.capacity) {
            if (other.ptr) {
                std::cout << "Konstruktor kopiujący - skopiowano\n";
                ptr = new T[size_val];
                for (int i = 0; i < size_val; i++) {
                    ptr[i] = other.ptr[i];
                }
            } else {
                ptr = nullptr;
            }
        }
        
        // Operator przypisania kopiujący
        vector& operator=(const vector<T>& other) {
            if (this != &other) {
                std::cout << "Operator przypisania - skopiowano\n";
                delete[] ptr;
                size_val = other.size_val;
                capacity = other.capacity;
                ptr = new T[size_val];
                for (int i = 0; i < size_val; i++) {
                    ptr[i] = other.ptr[i];
                }
            }
            return *this;
        }
        
        // Konstruktor przenoszący
        vector(vector<T>&& other) noexcept
            : ptr(other.ptr), size_val(other.size_val), capacity(other.capacity) {
            std::cout << "Konstruktor przenoszący - przeniesiono\n";
            other.ptr = nullptr;
            other.size_val = 0;
            other.capacity = 0;
        }
        
        // Operator przypisania przenoszący
        vector& operator=(vector<T>&& other) noexcept {
            if (this != &other) {
                std::cout << "operator przypisania - przeniesiono\n";
                delete[] ptr;
                ptr = other.ptr;
                size_val = other.size_val;
                capacity = other.capacity;
                other.ptr = nullptr;
                other.size_val = 0;
                other.capacity = 0;
            }
            return *this;
        }
        
        // Destruktor
        ~vector() {
            std::cout << "Pogrzebane\n";
            delete[] ptr;
        }

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


        int size() const {
            return size_val;  
        }

        const T& operator[](std::size_t idx) const {
            if (idx >= size_val) {
                throw std::out_of_range("Indeks poza zakresem");
            }
            return ptr[idx];
        }

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
        
        //Zadanie 2
        template <typename... Args>
        void emplace_back(Args&&... args) {
            if (size_val == capacity) {
                resize();  
            }
            new (&ptr[size_val]) T(std::forward<Args>(args)...);  
            size_val++; 
        }
    };
}

struct pixel {
    int r, g, b;

    pixel() : r(0), g(0), b(0) {}

    pixel(int r, int g, int b) noexcept : r(r), g(g), b(b) {}

    pixel(const pixel&) = default;
    pixel(pixel&&) noexcept = default;
    pixel& operator=(const pixel&) = default;
    pixel& operator=(pixel&&) noexcept = default;

    ~pixel() = default;
};

int main() {
    // 1. Konstruktor domyślny
    cpplab::vector<int> v1;
    
    v1.add({1, 2, 3, 4, 5});

    std::cout << "V1: ";
    for (int i = 0; i < v1.size(); i++) {
        std::cout << v1[i] << "\t";
    }
    std::cout << "\n";

    // 2. Konstruktor kopiujący
    cpplab::vector<int> v2 = v1;
    
    std::cout << "V2: ";
    for (int i = 0; i < v2.size(); i++) {
        std::cout << v2[i] << "\t";
    }
    std::cout << "\n";

    // 3. Operator przypisania kopiujący
    cpplab::vector<int> v3;
    
    v3 = v1;
    std::cout << "V3: ";
    for (int i = 0; i < v3.size(); i++) {
        std::cout << v3[i] << "\t";
    }
    std::cout << "\n";

    // 4. Konstruktor przenoszący
    cpplab::vector<int> v4 = std::move(v1);
    
    std::cout << "V4: ";
    for (int i = 0; i < v4.size(); i++) {
        std::cout << v4[i] << "\t";
    }
    std::cout << "\n";

    // 5. Operator przypisania przenoszący
    cpplab::vector<int> v5;
    
    v5 = std::move(v2);
    std::cout << "V5: ";
    for (int i = 0; i < v5.size(); i++) {
        std::cout << v5[i] << "\t";
    }
    std::cout << "\n\n";
    
    
    //zadanie 2
    std::cout << "Zadanie 2\n";
    
    cpplab::vector<pixel> pixels;

    pixels.emplace_back(3, 4, 6);  

    std::cout << "Pixel: " << pixels[0].r << ", " 
              << pixels[0].g << ", " 
              << pixels[0].b << "\n";
    
    std::cout << "\n\n";


    return 0;
}

