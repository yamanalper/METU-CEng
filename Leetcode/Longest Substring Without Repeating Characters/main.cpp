class Solution {
public:
    bool contains(vector<char> v, char c){
        for (int i = 0; i < v.size(); i++){
            if (v[i] == c) return true;
        }
        return false;
    }

    int lengthOfLongestSubstring(string s) {
        int count=0,i;
        vector<char> subs;
        for( i = 0; i<s.size(); i++){
            int k = i;
            while(!contains(subs , s[k])){
                subs.push_back(s[k]);
                if(k<s.size() - 1) k++;
            }
            if(count < subs.size() ) count = subs.size();
            subs.clear();
        }
        return count;
    }
};
