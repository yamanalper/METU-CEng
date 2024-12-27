#include <iostream>
#include <fstream>
#include "the5.h"


void test(){
    char inp_file[] = "/Users/alper/Desktop/test/test/the5/the5/THE5/inp09.txt"; // 01 to 10 is available
    int size;
    std::ifstream infile (inp_file);
    if(!infile.is_open()){
        std::cout << "Input file cannot be opened" << std::endl;
        std::cout << "File name: " << inp_file << std::endl;
        return;
    }
    infile >> size;
    std::vector<std::vector<bool>> dependency_graph(size, std::vector<bool>(size, false));
    for(int idx=0; idx < size ; idx++)
        for(int idy=0; idy < size; idy++){
            int temp; infile >> temp;
            dependency_graph[idx][idy] = (temp != 0);
        }

    std::cout << "Number of work items: " << size << std::endl <<
                 "Dependency matrix: [" << std::endl;
    for(int idx=0; idx < size; idx++){
        std::cout << "                        [";
        for(int idy=0; idy < size; idy++)
            std::cout << dependency_graph[idx][idy] << (idy != size - 1 ? ", " : "]");
        std::cout << std::endl;
    }
    std::cout << "                   ]"<< std::endl;

    std::vector<std::vector<int> > work_order = find_work_order(dependency_graph);

    std::cout <<"Sorted order: ";
    int n_of_super_node = (int)work_order.size();
    for(int idx=0; idx < n_of_super_node; idx++) {
        std::cout << "[";
        for(int idy=0; idy < (int)work_order[idx].size(); idy++)
            std::cout << work_order[idx][idy] << ((idy != (int)work_order[idx].size() - 1) ? ", " : "]" );
        if(idx != n_of_super_node-1) std::cout << " -> ";
    }

    std::cout << std::endl << "--------------------------------------------" << std::endl;
    infile.close();
}

int main(){
    test();
    return 0;
}
