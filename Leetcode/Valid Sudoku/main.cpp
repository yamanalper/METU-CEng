
class Solution {
public:
    bool duplicate(vector<char> v, char c){
        int count = 0;
        for (int i = 0; i < v.size(); i++){
            if (v[i] == c) count++;
        }
        if (count == 1) return false;
        return true;
    }

    bool check_v(vector<vector<char>>& board){
        int count = 0;
        vector<char> vert;
        for(int i = 0 ; i<9 ; i++){
            for(int j = 0; j<9; j++){
                vert.push_back(board[j][i]);
            }
            for(int j = 0; j<9; j++){
                if(vert[j] != '.' && duplicate(vert, vert[j]) ) return false;
            }
            vert.clear();
        }
        return true;
    }


    bool isValidSudoku(vector<vector<char>>& board) {
        int i,j,k;
        vector<char> squ;
        for(i = 0; i<9; i++){
            for(j = 0; j<9 ; j++){
                if(board[i][j] != '.' && duplicate(board[i] , board[i][j])) return false;
            }
        }
        for(i = 0; i < 9; i+=3){
            for(j = 0; j<9 ; j+= 3){
                squ.push_back(board[i][j]);
                squ.push_back(board[i][j+1]);
                squ.push_back(board[i][j+2]);
                squ.push_back(board[i+1][j]);
                squ.push_back(board[i+1][j+1]);
                squ.push_back(board[i+1][j+2]);
                squ.push_back(board[i+2][j]);
                squ.push_back(board[i+2][j+1]);
                squ.push_back(board[i+2][j+2]);
                for(k = 0; k<squ.size(); k++){
                    if (squ[k] != '.' && duplicate(squ , squ[k])) return false;
                }
                squ.clear();
            }
        }
        if(!check_v(board)) return false;
        return true;
    }
};
