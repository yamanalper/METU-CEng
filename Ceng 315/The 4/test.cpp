#include <iostream>
#include <chrono>
#include <ctime>
#include "the4.h"

const unsigned short CAKE_LIMIT = 600;

void random_input_generator(unsigned short &cake_width,
                            unsigned short &cake_height,
                            bool **&perfect_cuts){
    float prob_of_perfect_cuts = (float)(random() % 10000) / 10000.0 / 900;
    cake_width = random() % CAKE_LIMIT;
    cake_height = random() % CAKE_LIMIT;
    perfect_cuts = new bool*[cake_width+1];

    for(int idx=0; idx < cake_width+1; idx++){
        perfect_cuts[idx] = new bool[cake_height+1];
        for(int idy=0; idy < cake_height+1; idy++)
            perfect_cuts[idx][idy] = idx != 0 and idy != 0 and ((float)(random() % 10000) / 10000.0) < prob_of_perfect_cuts;
        
    }
}


void test(){
    unsigned short cake_width, cake_height;
    unsigned int wasted_cake_area;
    bool** perfect_cuts;
    unsigned int number_perfect_cuts = 0;
    random_input_generator(cake_width, cake_height, perfect_cuts);

    std::cout << "Width: " << cake_width << " - Height: " << cake_height << std::endl <<
              "Perfect cuts: {";
    for(int idx = 0; idx < cake_width; idx++)
        for(int idy = 0; idy < cake_height; idy++)
            if(perfect_cuts[idx][idy]){
                std::cout << "(" << idx << ", " << idy << ") ";
                number_perfect_cuts++;
            }
    std::cout << "}" << std::endl <<
              "Number of perfect cuts: " << number_perfect_cuts << std::endl << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();
    wasted_cake_area = alice_cutting_cake(cake_width, cake_height, perfect_cuts);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Wasted cake area: " << wasted_cake_area << std::endl
              << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" << std::endl;

    for(int idx=0; idx < cake_width; idx++)
        delete[] perfect_cuts[idx];
    delete[] perfect_cuts;

}


int main(){
    srandom(time(nullptr));
    test();
    return 0;
}