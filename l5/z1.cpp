#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

// Zadanie 1: Implementacja BST przy użyciu std::unique_ptr
template <typename T>
class BST {
private:
    struct Node {
        T value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node* parent;

        Node(T val, Node* par = nullptr)
            : value(val), parent(par), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    void print_in_order(const Node* node) const {
        if (!node) return;
        print_in_order(node->left.get()); // Left
        std::cout << node->value << " "; // Root
        print_in_order(node->right.get()); // Right
    }

public:
    BST() : root(nullptr) {}

    void insert(const T& value) {
        if (!root) {
            root = std::make_unique<Node>(value);
            return;
        }

        Node* current = root.get();
        Node* parent = nullptr;

        while (current) {
            parent = current;
            if (value < current->value) {
                current = current->left.get();
            } else {
                current = current->right.get();
            }
        }

        if (value < parent->value) {
            parent->left = std::make_unique<Node>(value, parent);
        } else {
            parent->right = std::make_unique<Node>(value, parent);
        }
    }

    void print_in_order() const {
        print_in_order(root.get());
        std::cout << std::endl;
    }
};

// ZADANIE 2a: unique_ptr
template <typename T>
class UniquePtr {
private:
    T* ptr; 

public:
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    ~UniquePtr() {
        delete ptr; 
    }

    UniquePtr(const UniquePtr&) = delete; // Wyłączenie kopiowania
    UniquePtr& operator=(const UniquePtr&) = delete; // Wyłączenie przypisywania

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; } 
    T* operator->() const { return ptr; } // Dostęp do metod obiektu

    T* get() const { return ptr; }
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

// ZADANIE 2b: Implementacja wskaźnika non0_ptr 
template <typename T>
class Non0Ptr {
private:
    T* ptr;

public:
    explicit Non0Ptr(T* p) : ptr(p) {
        if (!ptr) {
            throw std::invalid_argument("Non0Ptr nie może być zainicjalizowany nullptr.");
        }
    }

    ~Non0Ptr() = default;

    Non0Ptr(const Non0Ptr&) = delete; // Wyłączenie kopiowania
    Non0Ptr& operator=(const Non0Ptr&) = delete; // Wyłączenie przypisywania

    Non0Ptr(Non0Ptr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    Non0Ptr& operator=(Non0Ptr&& other) noexcept {
        if (this != &other) {
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; } 
    T* operator->() const { return ptr; } // Dostęp do metod obiektu
};

// ZADANIE 3: Problem plecakowy (Knapsack)
struct Item {
    int value;  // Wartość przedmiotu
    int weight; // Waga przedmiotu
};


int knapsack(int capacity, const std::vector<Item>& items) {
    int n = items.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = std::max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][capacity];
}

// Testy
int main() {
    // Zadanie 1: Testowanie drzewa BST
    std::cout << "Zadanie 1: Testowanie BST\n";
    BST<int> tree;
    tree.insert(10);
    tree.insert(15);
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);
    std::cout << "Posortowane wartości w drzewie: ";
    tree.print_in_order();

    // Test UniquePtr
    UniquePtr<int> uptr(new int(42));
    std::cout << "Wartość UniquePtr: " << *uptr << std::endl;

    UniquePtr<int> uptr2 = std::move(uptr);
    if (!uptr.get()) {
        std::cout << "uptr jest teraz nullptr po przeniesieniu." << std::endl;
    }

    // Test Non0Ptr
    try {
        Non0Ptr<int> nptr(new int(10));
        std::cout << "Wartość Non0Ptr: " << *nptr << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Test problemu plecakowego
    std::vector<Item> items = {{1, 4}, {2, 5}, {3, 6}};
    int capacity = 12;
    std::cout << "Maksymalna wartość plecaka: " << knapsack(capacity, items) << std::endl;

    return 0;
}
