#include "the2.h"

// DO NOT CHANGE ABOVE THIS LINE!
// you may implement helper functions below


template <class T> const T& max (const T& a, const T& b) {
  return (a<b)?b:a;     // or: return comp(a,b)?b:a; for version (2)
}

void counting_sort(std::string *arr, const int size, const int group_size, int start, int &count, bool ascending){

    int count_size = int( pow(27 , group_size) );
    int count_array[count_size];
    int temp = 0;
    std::string output[size];

    for (int i = 0; i < count_size; i++) {
        count_array[i] = 0;
    }

    for(int i = 0; i < size; i++){
        for (int j = 0; j < group_size; j++){
            if (arr[i].length() <=     start + j ) temp += pow(27, group_size - j - 1);
            else temp += ( int ( arr[i][start + j]) - 65 ) * pow(27, group_size - j - 1);
        }
        count++;
        count_array[temp]++;
        temp = 0;
    }


    if(ascending == true){
        for (int i = 1; i < count_size; i++){
            count_array[i] = count_array[i] + count_array[i-1];
            count++;
        }
    }
    else{
        for (int i = count_size-2; i >= 0; i--){
            count_array[i] = count_array[i] + count_array[i+1];
            count++;
        }
    }
    

    for(int i = size - 1 ; i >= 0; i--){
        for (int j = 0; j < group_size; j++){
            if (arr[i].length() <= start + j ) temp += pow(27, group_size - j - 1);
            else temp += ( int ( arr[i][start + j]) - 65 ) * pow(27, group_size - j - 1);
        }
        output[count_array[temp]-1] = arr[i];
        if(ascending == true) count_array[temp]--;
        else count_array[temp]--;
        count++;
        temp = 0;
    }
    
    for (int i = 0; i < size; i++) {
            arr[i] = output[i];
            count++;
    }


}



long multi_digit_string_radix_sort(std::string *arr,
                                   const int size,  // length of arr
                                   const bool ascending,  // alphabetical order or not
                                   const int group_size){  // bucket partition size
    // your code here
    
    int count = 0;
    int max_length = arr[0].length();
    

    for(int i = 1 ; i < size; i++){
        max_length = max( int(arr[i].length()), max_length);
    }

    for (int i = max_length - group_size; i >= 0; i -= group_size) {
        if ( i % group_size != 0 && i < group_size ) counting_sort(arr, size, i%group_size, i, count, ascending);
        else counting_sort(arr, size, group_size, i, count, ascending);
    }
    
    


    
    return count;
}
