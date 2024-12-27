#include <iostream>
#include <fstream>
#include "the6.h"


void print_network(std::vector<std::vector<std::pair<int,int>>>& network) {
    int node_number = (int) network.size();
    if (node_number == 0) {
        std::cout << "There is no node in the network" << std::endl;
        return;
    }

    for (int idx=0; idx < node_number; idx++) {
        std::cout << idx << ":\t{";
        for (const auto& edge : network[idx]) {
            std::cout << " (" << edge.first << ", " << edge.second << ") ";
        }
        std::cout << "} " << std::endl;
    }
}

void read_from_file(std::vector<std::vector<std::pair<int, int>>>& network){
    int node_number, edge_number;
    char addr[]= "/Users/alper/Downloads/the6_io/inp02.txt"; // 01-10 are available
    std::ifstream infile (addr);
    if (!infile.is_open()){
        std::cout << "File \'"<< addr
                  << "\' can not be opened. Make sure that this file exists." << std::endl;
        return;
    }
    infile >> node_number >> edge_number;
    network.resize(node_number);
    for(int idy=0; idy < edge_number; idy++) {
        int source, dest, weight;
        infile >> source >> dest >> weight;
        network[source].push_back(std::make_pair(dest, weight));
    }
    infile.close();
}

int main(){
    std::vector<std::vector<std::pair<int, int>>> network;
    read_from_file(network);
    print_network(network);
    std::vector<std::vector<float> > friendship_scores = get_friendship_scores(network);
    int N = (int)friendship_scores.size();
    std::cout << "Friendship scores formatted as 'FS(<node1>,<node2>) = <score>': " << std::endl;
    for(int idx=0; idx < N; idx++){
        for(int idy=0; idy <= idx; idy++){
            std::cout << "FS(" << idx << "," << idy << ") = " << friendship_scores[idx][idy] << std::endl;
        }
    }
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}
