#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;


template <class T>
class Trie {
private:
    // Define TrieNode for the first phase (Trie)
    /* DO NOT CHANGE */
    struct TrieNode {
        static const int ALPHABET_SIZE = 128;
        TrieNode* children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T* data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(NULL) {

                for (int i = 0; i< ALPHABET_SIZE; i++) {
                children[i] = NULL;
                
            }
            
            }
    };

    TrieNode* root;

public: // Do not change.

    Trie();
    ~Trie();
    
    Trie& insert(const string& username);
    void remove(std::string username);
    T* search(std::string username);
    void findStartingWith(std::string prefix, std::vector<T*> &results);
    void wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results);
    void print();

private: // you may add your own utility member functions here.
    void wildcardHelper(TrieNode* root, const std::string& wildcardKey, std::vector<T*>& results, size_t indx, const std::string& currKey);
    void findAllKeys(TrieNode* root ,vector<T*> &results);
    void print(const std::string& primaryKey); // Do not change this line.
    void printTrie(TrieNode* node, const std::string& currentKey); // Do not change this line.
    void deleteTrieNode(TrieNode* &root);
    
};
/* DO NOT CHANGE */
template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
void Trie<T>::findAllKeys(TrieNode* root, vector<T*> &results){
    
    if(root == NULL) return;
    
    if(root->isEndOfKey == true) results.push_back(root->data);
    
    for(int i = 0 ; i<128 ; i++){
        
        if(root->children[i]) findAllKeys(root->children[i],results);
    }
}


template <class T>
void Trie<T>::deleteTrieNode(TrieNode* &root){
    
    if(root){
        
        for(int i = 0; i<128 ; i++){
            if(root -> children[i] != NULL ) {
                delete root -> children[i] -> data;
                deleteTrieNode(root -> children[i]);
            }
        }
        delete root;
        
    }
    
}

template <class T>
Trie<T>::~Trie() {
    deleteTrieNode(root);
}

template <class T>
Trie<T>& Trie<T>::insert(const string& key) {

    TrieNode* curr = root;
    int i = 0;
    
    for (i = 0 ; i < key.size() ; i++){
        
        if (curr->children[ int(key[i])]  == NULL){
            
            curr->children[ int(key[i]) ] = new TrieNode(key[i]);
            curr = curr->children[ int(key[i]) ];
        }
        
        else{
            curr = curr->children[ int(key[i]) ];
        }
        
    }
    
    curr->isEndOfKey = true;
    curr->data = new T(key);
    
    
    return *this;
}

template <class T>
T* Trie<T>::search(std::string username) {
    
    TrieNode* curr = root;
    
    for(int i = 0; i < username.size() ; i++ ){
        
        if(curr->children[int(username[i])] != NULL) curr = curr->children[int(username[i])];
        
        else return NULL;
        
        if(i == username.size()-1 && curr -> isEndOfKey == true) return curr->data;
        
    }
    
    return NULL;
    
}

template <class T>
void Trie<T>::remove(std::string username) {
    
    TrieNode* curr = root;

    for(int i = 0; i < username.size() ; i++ ){
    
        if(curr->children[int(username[i])] != NULL) curr = curr->children[int(username[i])] ;
        
        
        if(i == username.size()-1 && curr->isEndOfKey == true) curr->isEndOfKey = false;
    }
    
    
    
}

template <class T>
void Trie<T>::findStartingWith(string prefix,vector<T*> &results) {

    TrieNode* curr = root;
    int i;
    
    for (i = 0; i < prefix.size(); i++){
        
        if(curr->children[int(prefix[i])] != NULL) curr = curr->children[int(prefix[i])] ;
        
        else return;
        
        if(i == prefix.size()-1){
            findAllKeys(curr , results);
        }
    }
}


template <class T>
void Trie<T>::wildcardHelper(TrieNode* root, const std::string& wildcardKey, std::vector<T*>& results, size_t indx, const std::string& currKey){
    
    if(indx == wildcardKey.size()){
        if(root->isEndOfKey && root->data) results.push_back(root->data);
        return;
    }
    
    char curr = wildcardKey[indx];
    
    if(curr == '*'){
        if(wildcardKey[indx+1] == '*'){
            wildcardHelper(root, wildcardKey, results, indx + 1, currKey);
            return;
        }
        wildcardHelper(root, wildcardKey, results, indx + 1, currKey);
        for(int i = 0 ; i<128 ; i++){
            if(root->children[i]){
                wildcardHelper(root->children[i], wildcardKey, results, indx, currKey + char(i));
            }
        }
    }
    else if (curr == '?'){
        for(int i = 0 ; i<128 ; i++){
            if(root->children[i]){
                wildcardHelper(root->children[i], wildcardKey, results, indx+1, currKey + char(i));
            }
        }
    }
    else{
        char match = curr;
        int childIndx = int(match);
        if(root->children[childIndx]){
             wildcardHelper(root->children[childIndx], wildcardKey, results, indx+1, currKey + match);
        }
    }
}


template <class T>
void Trie<T>::wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results) {
        
    wildcardHelper(root, wildcardKey, results , 0 , "");
    
    for(int i = 1; i < results.size()  ; i++){
        for(int k = 0 ; k < i ; k++){
            if(results[i] == results[k]){
                results.erase(results.begin() + i);
                i--;
                break;
            }
        }
        
    }
}

/* DO NOT CHANGE */
template<class T>
void Trie<T>::print() {
    print("");
}
/* DO NOT CHANGE */
template <class T>
void Trie<T>::print(const std::string& primaryKey) {
    if (primaryKey.empty()) {
        // Print the entire tree.
        printTrie(root, "");
    } else {
        // Print specific keys.
        TrieNode* primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++) {
            
            int index = primaryKey[i];
            if (!primaryNode->children[index]) {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return ;
            }
            primaryNode = primaryNode->children[index];
        }

        }
    }
/* DO NOT CHANGE */
template <class T>
void Trie<T>::printTrie(TrieNode* node, const std::string& currentKey) {
    if (!node) {
        return;
    }
    if (node->isEndOfKey) {
        std::cout << currentKey <<std::endl;
        std::cout << "Watched Movies:" << std::endl;
        if(node->data) {
            node->data->printMovies();
        }
    }
    for (int i = 0; i < 128; ++i) {
        if (node->children[i]) {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}

