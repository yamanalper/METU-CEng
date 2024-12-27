#include "the1.h"
#include <iostream>

// DO NOT CHANGE ABOVE THIS LINE!
// you may implement helper functions below

void partition(unsigned short *arr, const unsigned int size, long &swap, int &mark1, int &mark2){
                                                    int i = 1;
                                                    int j = 1;
                                                    int k = size-1;
                                                    int pivot = arr[0];
                                                    while (j <= k){
                                                        if (arr[j] == pivot){
                                                            std::cout << "Size: " << size << std::endl <<
                                                                         "Array elements: {";
                                                            for(int idx=0; idx < size; idx++) std::cout << arr[idx] << (idx != size-1 ? ", " : "}");
                                                            std::cout << std::endl;
                                                            j++;
                                                        }
                                                        else if (arr[j] < pivot){
                                                            std::swap(arr[i] , arr[j]);
                                                            std::cout << "Size: " << size << std::endl <<
                                                                         "Array elements: {";
                                                            for(int idx=0; idx < size; idx++) std::cout << arr[idx] << (idx != size-1 ? ", " : "}");
                                                            std::cout << std::endl;
                                                            swap++;
                                                            i++;
                                                            j++;
                                                        }
                                                        else if(arr[j] > pivot){
                                                            std::swap(arr[k] , arr[j]);
                                                            std::cout << "Size: " << size << std::endl <<
                                                                         "Array elements: {";
                                                            for(int idx=0; idx < size; idx++) std::cout << arr[idx] << (idx != size-1 ? ", " : "}");
                                                            std::cout << std::endl;
                                                            swap++;
                                                            k--;
                                                        }
                                                    }
                                                    
                                                    std::swap(arr[i-1] , arr[0]);
    std::cout << "Size: " << size << std::endl <<
                 "Array elements: {";
    for(int idx=0; idx < size; idx++) std::cout << arr[idx] << (idx != size-1 ? ", " : "}");
    std::cout << std::endl;
                                                    swap++;
                                                    mark1 = i-1;
                                                    mark2 = j;
                                                           
}

void quickk(unsigned short *arr, const unsigned int size, long &swap, int &call){

                if(size <= 1){
                    return;
                }

                int i,j;
                
                partition(arr,size,swap,i,j);
                
                quickk(arr,i,swap,call);
                call++;
                quickk( arr + j , size - j , swap, call);
                call++;
                
}

void select(unsigned short *arr, const unsigned int size, const unsigned int index, long &swap, int &call){

    if (size <= 1) return;

    int i,j;
    call++;
    partition(arr, size, swap, i, j);
    std::cout << "Size: " << size << std::endl <<
                 "Array elements: {";
    for(int idx=0; idx < size; idx++) std::cout << arr[idx] << (idx != size-1 ? ", " : "}");
    std::cout << std::endl;



    if (index < i){
        select(arr, i, index, swap, call);
    }
    else if(index >= i && index < j ){
        return;
    }
    else{
        select(arr + j, size - j, index - j, swap, call);
    }


}
            

std::pair<unsigned short, unsigned int> select_k_with_quick_sort3(unsigned short *arr,
                                                                  const unsigned int size,
                                                                  const unsigned int index,
                                                                  long &swap){
    // your code here
    int call = 0;
    
    quickk(arr,size,swap,call);
    call++;
    
    // do not forget to update the return value!
    return std::make_pair(arr[index], call);
}


std::pair<unsigned short, unsigned int> quick_select3(unsigned short *arr,
                                                      const unsigned int size,
                                                      const unsigned int index,
                                                      long &swap){
    // your code here
    int call = 0;

    select(arr, size, index, swap, call);
    
    // do not forget to update the return value!
    return std::make_pair(arr[index], call);
}
