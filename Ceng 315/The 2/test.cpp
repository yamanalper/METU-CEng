#include <iostream>
#include <fstream>
#include <chrono>
#include "the2.h"

void file_input(std::string *&input_array,
                int &size,
                bool &ascending,
                int &group_size){
        ascending = 1;            // Equivalent to reading '1'
        group_size = 3;           // Equivalent to reading '2'
        size = 11;                 // Equivalent to reading '5'

        // Allocate memory for the array
        input_array = new std::string[size];

        // Manually set each element in the array
    input_array[0] = "NEWTON";
    input_array[1] = "EINSTEIN";
    input_array[2] = "DIRAC";
    input_array[3] = "MAXWELL";
    input_array[4] = "FARADAY";
    input_array[5] = "BOHR";
    input_array[6] = "CURIE";
    input_array[7] = "FEYNMAN";
    input_array[8] = "PLANCK";
    input_array[9] = "BORN";
    input_array[10] = "HAWKING";
}

void test(){
    int size, group_size;
    long number_of_iteration;
    bool ascending;
    std::string *input_array;
    file_input(input_array, size, ascending, group_size);
    std::cout << "Size: " << size << std::endl <<
                 "Ascending: " << ascending << std::endl <<
                 "Group size: " << group_size << std::endl <<
                 "Array elements: {";
    for(int idx=0; idx < size; idx++)
        std::cout << input_array[idx] << (idx != size-1 ? ", "
                                                        : "}");
    std::cout << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();
    number_of_iteration = multi_digit_string_radix_sort(input_array, size, ascending, group_size);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Number of iterations: " << number_of_iteration << std::endl
              << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl
              << "Sorted array: {";
    for(int idx=0; idx < size; idx++)
        std::cout << input_array[idx] << (idx != size-1 ? ", " : "}");
    std::cout << std::endl;

}

int main(){
    test();
    return 0;
}
