#include <iostream>
#include <vector>
#include <initializer_list> 
//zadanie 1
template <typename T, typename C>
    T my_max(T a, T b, C comp){

        return comp(a,b) ? b:a;
    }

//zadanie 2
template <typename T>
    void insertion_sort(std::vector<T>& arr){
        int s = arr.size();

        for(int i = 1; i < s; i++){
            int k = i;
                
            while(arr[k] < arr[k-1] && k != 0){
                std::swap(arr[k],arr[k-1]);
                k--;
                }
            }
        
        for(int i = 0; i < arr.size(); i++){
        std::cout<<arr[i]<<'\t';
    }
    std::cout<<std::endl;

    }


int main(){
    //zadanie 1
    auto comp = [](auto a, auto b) { return a < b;};

    int x = 5, y = 6;
    char a = 'a', b = 'b';
    std::string jud = "judasz", bru = "brutus";
    double d = 3.141592'653589, t = 5.7;
    
    
    std::cout<< my_max(x, y,comp)<<std::endl;
    std::cout<< my_max(a, b,comp)<<std::endl;
    std::cout<< my_max(jud, bru,comp)<<std::endl;
    std::cout<< my_max(d, t,comp)<<std::endl;

    //zadanie 2
    std::vector<int> intig = {5,6,7,0,0,9,11,5,0};
    std::vector<std::string> str = {"a","0","wojtyła","podsiadło","Podsiadło"};
    std::vector<float> flo = {3.141592, 1.69, 2.71828, 1.61803, 1.4142};
    
    insertion_sort(intig);
    insertion_sort(str);
    insertion_sort(flo);
    return 0;
}