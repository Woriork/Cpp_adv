#include <iostream>
#include <vector>
#include <memory>
#include <string>

template <typename T>
class forward_list {
private:
    struct Node {
        T value;
        std::unique_ptr<Node> next;

        Node(T val) : value(val), next(nullptr) {}
    };

    std::unique_ptr<Node> head;

public:
    forward_list() : head(nullptr) {}

    void push(T data) {
        auto new_node = std::make_unique<Node>(data);
        if (head) {
            new_node->next = std::move(head);
        }
        head = std::move(new_node);
    }

    void reverse() {
        std::unique_ptr<Node> prev = nullptr;
        auto current = std::move(head);

        while (current) {
            auto next = std::move(current->next);
            current->next = std::move(prev);
            prev = std::move(current);
            current = std::move(next);
        }

        head = std::move(prev);
    }

    void print() const {
        auto* current = head.get();
        while (current) {
            std::cout << current->value << " ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};
int main() {
    forward_list<std::string> v_test;
    
    v_test.push("zupa");
    v_test.push("kura");
    v_test.push("jajko");
    v_test.push("arbuz");
    v_test.push("babilon");

    std::cout << "Original list:" << std::endl;
    v_test.print();

    v_test.reverse();

    std::cout << "Reversed list:" << std::endl;
    v_test.print();
    return 0;
}
