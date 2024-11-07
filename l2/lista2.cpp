#include <iostream>
#include <vector>
#include <algorithm>


int extractNumber(const std::string& str, int & pos) {
    int number = 0;
    while (pos < str.size() && std::isdigit(str[pos])) {
        number = number * 10 + (str[pos] - '0');
        pos++;
    }
    return number;
}

bool naturalCompare(const std::string& a, const std::string& b) {
    int i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        // Porównanie liczb
        if (std::isdigit(a[i]) && std::isdigit(b[j])) {

            int numA = extractNumber(a, i), numB = extractNumber(b, j);
            if (numA != numB) return numA < numB;
        }
        // Porównanie reszty str
        else if (a[i] != b[j]) {
            return a[i] < b[j];
        } else {
            i++;
            j++;
        }
    }
    return a.size() < b.size();
}


template <typename T>
    void sort(std::vector<T>& arr){
        int s = arr.size();

        for(int i = 1; i < s; i++){
            int k = i;
                
            while (k != 0 && arr[k] < arr[k-1]){
                std::swap(arr[k],arr[k-1]);
                k--;
                }
            }
        
        for(int i = 0; i < arr.size(); i++){
        std::cout<<arr[i]<<'\t';
    }
    std::cout<<std::endl;}


void sort(std::vector<std::string>& vec) {
    std::sort(vec.begin(), vec.end(), naturalCompare);
}

//zadanie 2

template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

//zadanie 3

int main() {
    std::vector<std::string> vec = {"abcd9","abcd81","ab9c8","#a","1"};
    
    sort(vec);

    for (const auto& str : vec) {
        std::cout << str << " ";
    }

        constexpr int result = Factorial<5>::value;  
    std::cout << '\n' << result <<std::endl;

    return 0;
}
