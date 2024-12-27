#include "the6.h"

// do not add extra libraries here


std::vector< std::vector<float> > get_friendship_scores(const std::vector< std::vector< std::pair<int, int> > >& network){
    std::vector< std::vector<int> > Adj(network.size(), std::vector<int>(network.size() , INT_MAX));
    std::vector<std::vector<float> > scores((int)network.size(), std::vector<float>((int)network.size(), 1));
    
    for(int i = 0 ; i < network.size() ; i++){
        for (int k = 0 ; k < network[i].size() ; k++){
            Adj[i][network[i][k].first] = network[i][k].second;
        }
    };
    
    for (int  k = 0 ; k < Adj.size() ; k++){
        for (int i = 0 ; i < Adj.size() ; i++){
            if (Adj[i][k] == INT_MAX) continue;
            for (int j = 0 ; j < Adj.size() ; j++){
                if ( Adj[i][j] > Adj[i][k] + Adj[k][j]  && Adj[k][j] != INT_MAX) Adj[i][j] = Adj[i][k] + Adj[k][j] ;
            }
        }
    }
    
    
    for (int i = 0; i < Adj.size() ; i++) {
        for (int k = 0; k <= i; k ++) {
            if( ( Adj[i][k] == INT_MAX && Adj[k][i] != INT_MAX ) || ( Adj[i][k] != INT_MAX && Adj[k][i] == INT_MAX ) ) scores[i][k] = 0;
            else if ( Adj[i][k] == INT_MAX && Adj[k][i] == INT_MAX ) scores[i][k] = -1;
            else{
                float minI = *min_element( Adj[i].begin(), Adj[i].end() ) ;
                float minK = *min_element( Adj[k].begin(), Adj[k].end() ) ;
                scores[i][k] = ( minI * minK ) / ( Adj[i][k] * Adj[k][i] ) ;
            }
        }
    }
    
    return scores;
}
