#include "the5.h"





std::vector<int> getOrder(const std::vector<int> &finish){
    std::vector<int> verticies(finish.size());
    for (int i = 0; i< finish.size(); i++) verticies[i] = i;

    for(int i = 0; i < verticies.size() - 1 ; i++){
        int maxIndex = i;
        for (int j = i+1 ; j < verticies.size(); j++ ){
            if (finish[verticies[j]] > finish[verticies[maxIndex]]) {
                            maxIndex = j;
                        }
        }
        std::swap(verticies[i], verticies[maxIndex]);
    }

    return verticies;
}

void DFSvisit(const std::vector< std::vector<bool> > &adj,std::vector<int> &color, std::vector<int> &depth, std::vector<int> &finish, std::vector<int> &parent, int curr_vertex, int &time){
    color[curr_vertex] = 1; //gray
    time++;
    depth[curr_vertex] = time;

    for (int k = 0; k < adj[curr_vertex].size(); k++){
        if ( adj[k][curr_vertex] && color[k] == 0){
            parent[k] = curr_vertex;
            DFSvisit(adj, color, depth, finish, parent, k, time);
        }
    }
    finish[curr_vertex] = ++time ;
    color[curr_vertex] = 2; //black
}

void DFSvisitSCC(const std::vector< std::vector<bool> > &adj,std::vector<int> &color, int curr_vertex, std::vector<int> &item){
    color[curr_vertex] = 1; //gray
    item.insert(
                     std::lower_bound( item.begin(), item.end(), curr_vertex ),
                     curr_vertex
                    );
    for(int k = 0; k < adj[curr_vertex].size(); k++){
        if(adj[k][curr_vertex] && color[k] == 0) DFSvisitSCC(adj, color, k, item);
    }
    color[curr_vertex] = 2; //black
}

void DFS(const std::vector< std::vector<bool> > &adj, std::vector<int> &finish){
    std::vector<int> color(adj.size(),0); //white
    std::vector<int> depth(adj.size(),-1);
    std::vector<int> parent(adj.size(), -1);
    int time = 0;

    for(int i = 0; i<adj.size(); i++){
        if(color[i] == 0) DFSvisit(adj, color, depth, finish, parent, i, time);
    }
}

void DFS2(const std::vector< std::vector<bool> > &adj, const std::vector<int> &order, std::vector<std::vector<int>> &work_order){
    std::vector<int> color(adj.size(),0); //white
    for(int i = 0; i < order.size(); i++){
        int vertex = order[i];
        if (color[vertex] == 0){
            std::vector<int> item;
            DFSvisitSCC(adj, color, vertex, item);
            work_order.push_back(item);
        }
    }
    
    
    
}

void getTranspose(const std::vector< std::vector<bool> > &dependencies, std::vector< std::vector<bool> > &transpose){
    for (int i = 0 ; i < dependencies.size() ; i++){
        for(int j = 0; j < dependencies.size(); j++){
            transpose[j][i] = dependencies[i][j];
        }
    }
}



void reverseVector(std::vector<std::vector<int>> &vec) {
    for (int i = 0; i < vec.size() / 2; i++) {
        std::vector<int> temp = vec[i];
        vec[i] = vec[vec.size() - i - 1];
        vec[vec.size() - i - 1] = temp;
    }
}

std::vector< std::vector<int> > find_work_order(const std::vector< std::vector<bool> > &dependencies){
    std::vector< std::vector<int> > work_order;
    std::vector< std::vector<bool> > transpose (dependencies.size(), std::vector<bool>( dependencies.size(),false ) );
    std::vector<int> finish1 (dependencies.size(),-1);
    std::vector<int> order (dependencies.size(),-1);

    DFS(dependencies , finish1);
    getTranspose(dependencies, transpose);
    order = getOrder(finish1);
    DFS2(transpose, order, work_order);
    reverseVector(work_order);
    

    return work_order;
}
