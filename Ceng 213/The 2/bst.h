#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;

// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class BST {
private:
    // Define TreeNode for the second phase (Binary Search Tree)
    /* DO NOT CHANGE */
    struct TreeNode {
        std::string key;
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const std::string &k, const T& value) : key(k), data(value), left(NULL), right(NULL) {}
        
    };

    TreeNode* root;


public: // Do not change.

    BST();
    ~BST();
    TreeNode* getRoot() { return root; }
    bool isEmpty() { return root == NULL; }

    BST& insert(const std::string key, const T& value);
    bool search(std::string value) const;
    void remove(std::string value);
    BST<T>* merge(BST<T>* bst);
    BST<T>* intersection(BST<T>* bst);
    std::vector<TreeNode> tree2vector(TreeNode* root);
    void print();
    
private:// you may add your own utility member functions here.

    typename BST<T>::TreeNode* vector2tree(std::vector<TreeNode> vect, int start, int end);
    void removeRecursive(TreeNode* &root, std::string key);
    bool isLeaf(TreeNode* r);
    bool searchRecursive(const TreeNode* root, const std::string value) const;
    void insertRecursive(TreeNode* & root, TreeNode* add);
    void deleteRecursive(TreeNode* root);
    void print(TreeNode* node, std::string indent, bool last, bool isLeftChild); // Do not change.
    
};


    template <class T>
    bool BST<T>::isLeaf(TreeNode* r){
        if(r->left == NULL && r->right == NULL) return true;
        return false;
    }

    // Constructor
    template <class T>
    BST<T>::BST() : root(NULL) {}
    
    template <class T>
    void BST<T>::deleteRecursive(TreeNode* root){
        
        if(root != NULL){
            deleteRecursive(root->right);
            deleteRecursive(root->left);
            delete root;
        }
        
    }


    // Destructor
    template <class T>
    BST<T>::~BST() {
        TreeNode* curr = root;
        deleteRecursive(curr);
    }

    template <class T>
    void BST<T>::insertRecursive(TreeNode* & root, TreeNode* add){
        
        if(root == NULL){
            root = add;
        }
        else if (root->key > add->key) insertRecursive(root->left,add);

        else if(root->key < add->key) insertRecursive(root->right,add);

        
    }

    // Insert function for BST.
    template <class T>
    BST<T>& BST<T>::insert(const string key, const T& value) {
        TreeNode* add = new TreeNode(key,value);
        insertRecursive(root,add);
        return *this;
    }
    
    template <class T>
    bool BST<T>::searchRecursive(const TreeNode* root, const std::string value) const {
        
        if (root == NULL) return false;
        
        else if(value == root->key) return true;
        
        else if (value < root->key) return searchRecursive(root->left , value);
        
        else if (value > root->key) return searchRecursive(root->right, value);
        
        return false;
    }
    
    // Search function for BST.
    template <class T>
    bool BST<T>::search(std::string value) const {
        if (root != NULL) return searchRecursive(root , value);
        else return false;
    }
    

    template <class T>
    void BST<T>::removeRecursive(TreeNode *&root, std::string key){
        
        if(root == NULL) return;
        
        else if(root->key > key) removeRecursive(root->left, key);
        
        else if(root->key < key) removeRecursive(root->right, key);
        
        else{
            if(root->left != NULL && root->right != NULL){
                TreeNode* curr = root->right;
                while(curr->left != NULL){
                    curr = curr->left;
                }
                root->key = curr->key;
                root->data = curr->data;
                removeRecursive(root->right, root->key);
            }
            else{
                TreeNode* curr = root;
                root = (root->left != NULL)? root->left : root->right;
                delete curr;
            }
        }
    }
    
    // Remove a node from BST for given key. If key not found, do not change anything.
    template <class T>
    void BST<T>::remove(std::string key) {
        removeRecursive(root, key);
    }
    
    // A helper function for converting a BST into vector.
    template <class T>
    vector<typename BST<T>::TreeNode> BST<T>::tree2vector(TreeNode* root) {
        vector<TreeNode> result;
        if (root != NULL) {
        
            vector<TreeNode> leftSubtree = tree2vector(root->left);
            result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());
    
            
            result.push_back(*root);
    
            
            vector<TreeNode> rightSubtree = tree2vector(root->right);
            result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());
        }
        return result;
    }
    
    template <class T>
    typename BST<T>::TreeNode* BST<T>::vector2tree(std::vector<typename BST<T>::TreeNode> vect, int start, int end){

        if (start > end) return NULL;
       /*
        int mid = (start + end) / 2;
        TreeNode* root = new TreeNode(vect[mid].key , vect[mid].data);
        
        
        root->left = vector2tree(vect, start, mid-1);
        root->right = vector2tree(vect, mid+1, end);
        
        return root;
        */
        TreeNode* root = new  TreeNode(vect[0].key, vect[0].data);
        TreeNode* curr = root;
        for(int i = 1; i<end+1 ; i++){
        
            root->right = new TreeNode(vect[i].key, vect[i].data);
            root = root->right;
        }
        
        return curr;
    }


    // Merge two BST's and return merged BST.
    template <class T>
    BST<T>* BST<T>::merge(BST<T>* bst) {
        
        vector<TreeNode> lhs = tree2vector(root);
        vector<TreeNode> rhs = tree2vector(bst->root);
        vector<TreeNode> merged;
        int i = 0;
        int k = 0;
        BST<T>* mergedBst = new BST<T>();
        
        
        if(lhs.size() == 0) {
            mergedBst->root = vector2tree(rhs, 0, rhs.size()-1);
            return mergedBst;
        }
    
        else if(rhs.size() == 0) {
            mergedBst->root = vector2tree(lhs, 0, lhs.size()-1);
            return mergedBst;
        }
        else if(lhs.size() == 0 && rhs.size() == 0){
            mergedBst-> root = NULL;
            return mergedBst;
        }
        
        
        
        while(i<lhs.size() && k < rhs.size()){
            if(lhs[i].key < rhs[k].key){
                merged.push_back(lhs[i]);
                i++;
            }
            else if(lhs[i].key > rhs[k].key){
                merged.push_back(rhs[k]);
                k++;
            }
            else{
                merged.push_back(lhs[i]);
                i++;
                k++;
            }
        }
        while(i < lhs.size()){
            merged.push_back(lhs[i]);
            i++;
        }
        while(k < rhs.size()){
            merged.push_back(rhs[k]);
            k++;
        }
        
        
        
        mergedBst->root = vector2tree(merged, 0, merged.size()-1);
        return mergedBst;
        
        
        
    }
        
    // Intersect two BST's and return new BST.
    template <class T>
    BST<T>* BST<T>::intersection(BST<T>* bst) {
        
        vector<TreeNode> lhs = tree2vector(root);
        vector<TreeNode> rhs = tree2vector(bst->root);
        vector<TreeNode> intrsc ;
        int i = 0;
        int k = 0;
        BST<T>* intrscBst = new BST<T>();
        
        if(lhs.size() == 0 || rhs.size() == 0){
            intrscBst-> root = NULL;
            return intrscBst;
        }
        
        for(i = 0; i< lhs.size(); i++){
            for(k = 0; k<rhs.size(); k++){
                
                if(lhs[i].key == rhs[k].key){
                    intrsc.push_back(lhs[i]);
                    break;
                }
            }
        }
        
        intrscBst->root = vector2tree(intrsc, 0, intrsc.size()-1);
        return intrscBst;
        
        
        
        return this;
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print() {
        print(root, "", true, false);
    
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print(TreeNode* node, string indent, bool last, bool isLeftChild) {
        if (node != NULL) {
            cout << node->key << endl;
            print(node->left, indent, false, true);
            print(node->right, indent, true, false);
        }
    
    }


