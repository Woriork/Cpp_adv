#include <iostream>
#include <vector>
#include <string> 

template<typename T>
std::vector<T*> as_sorted_view(std::vector<T> &vec){
    std::vector<T*> V_ptrs;
    
    for(int i = 0; i < vec.size(); i++){
        V_ptrs.push_back(&vec[i]);
    }
    
    //bąbelkowane - sortowane :)
    bool swapped;
    int size = V_ptrs.size();
    do {
        swapped= false;
        for(int i = 1; i < size; i++){
            
            if(*V_ptrs[i - 1] > *V_ptrs[i]){
                std::swap(V_ptrs[i-1], V_ptrs[i]);
                swapped = true;
            }
        }
        size--;
    }   while(swapped);
    
    return V_ptrs;
}

int main() {
    std::vector<std::string> v_test = {"zupa","kura","jajko","arbuz","babilon"};
    auto v_output = as_sorted_view(v_test);
    
    for(auto& w : v_output){
        std::cout<<*w<<std::endl; 
        }
    std::cout<<std::endl;
    for(auto& v : v_test){
        std::cout<<v<<std::endl;
    }
    return 0;
}
