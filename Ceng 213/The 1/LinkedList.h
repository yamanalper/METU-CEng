#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &rhs);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);
    void mergeNodes(int sourceIndex, int destIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList()
{
    head = new Node<T>(T());
    size =  0;
    
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &rhs)
{
    head = new Node<T>(T());
    *this = rhs;
}

template<class T>
LinkedList<T>::~LinkedList()
{
    for( int i=0; i<size+1; i++){
        Node<T>* curr = head;
        head = head->next;
        delete curr;
    }
    size = 0;
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if(this == &rhs){
        return *this;
    }
    Node<T>* rhs_curr = rhs.head->next;
    Node<T>* curr = head;
    removeAllNodes();
    while(rhs_curr != NULL){
        Node<T>* add = new Node<T>(rhs_curr->data);
        curr->next = add;
        add->prev = curr;
        curr = curr->next;
        rhs_curr = rhs_curr->next;
        }
    size = rhs.size;
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    return size;
 }

template<class T>
bool LinkedList<T>::isEmpty() const
{
    if(head->next == NULL) return true;
    else return false;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    Node<T>* curr=head;
    while (curr != NULL){
        if(curr->data == node->data){
            if(curr->next == node->next && curr->prev == node->prev){
                return true;
            }
        }
        curr = curr->next;
    }
    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    Node<T>* curr=head->next;
    return curr;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    Node<T>* curr=head;
    if(curr->next == NULL){
        return NULL;
    }
    else{
        for (int i = 0; i<size ; i++){
            curr = curr->next;
        }
        return curr;
    }
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    Node<T>* curr=head;
    while(curr != NULL){
        if(curr->data == data){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    Node<T>* curr=head;
    if(index >= size || index < 0){
        return NULL;
    }
    else{
        for( int i = 0; i<index+1 ; i++){
            curr = curr->next;
        }
        return curr;
    }
    
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    Node<T>* curr=head;
    Node<T>* appnd = new Node<T>(data);
    while(curr->next != NULL){
        curr = curr->next;
    }
    appnd->prev = curr;
    appnd->next = NULL;
    curr->next = appnd;
    size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    Node<T>* add = new Node<T>(data);
    add->next =  head->next;
    add->prev = head;
    if(head->next != NULL){
        head->next->prev = add;
    }
    head->next = add;
    size++;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    Node<T>* curr=head;
    int indx;
    if(containsNode(node)){
        Node<T>* add = new Node<T>( data );
        indx = getIndex(node);
        for(int i=0; i<indx+1;i++){
            curr = curr->next;
        }
        add->next = curr->next;
        if(curr->next != NULL){
            curr->next->prev = add;
        }
        curr->next = add;
        add->prev = curr;
        size += 1;
    }
    
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    Node<T>* curr = head;
    
    if(index >= size || index < 0){
        return;
    }
    
    Node<T>* add = new Node<T>( data );
    
    for(int i = 0; i<index;i++){
        curr = curr->next;
    }
    add->next = curr->next;
    if(curr->next != NULL){
        curr->next->prev = add;
    }
    curr->next = add;
    add->prev = curr;
    size++;
    
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    Node<T>* curr = head;
    int count = 0;
    while( curr != NULL ){
        curr = curr->next;
        if(curr->data == node->data){
            if( (curr->prev == node->prev) && (curr->next == node->next) ){
                return count;
            }
        }
        count++;
    }
    return -1;
    
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    if(currentIndex >= size){
        return;
    }
    else{
        Node<T>* nw = head;
        Node<T>* curr = head;
        for( int i = 0; i<currentIndex+1; i++){
            curr = curr->next;
        }
        if(newIndex >= size-1){
            for(int i = 0; i<size ; i++){
                nw = nw->next;
            }
            if(curr->next != NULL){
                curr->next->prev = curr;
            }
            curr->prev->next = curr->next;
            nw->next = curr;
            curr->prev = nw;
            curr->next = NULL;
            
        }
        else{
            for( int i = 0; i<newIndex; i++){
                nw = nw->next;
            }
            if(curr->next != NULL){
                curr->next->prev = curr->prev;
            }
            curr->prev->next = curr->next;
            curr->next = nw->next;
            nw->next->prev = curr;
            nw->next = curr;
            curr->prev = nw;
        }
        
    }
}

template<class T>
void LinkedList<T>::mergeNodes(int sourceIndex, int destIndex)
{
    Node<T>* sour = head;
    Node<T>* dest = head;
    Node<T>* tmp;
    int i=0;
    if(sourceIndex >= size || sourceIndex < 0 || destIndex < 0 || destIndex >= size){
        return;
    }
    else{
        for(i=0;i<sourceIndex+1;i++){
            tmp = sour;
            sour = sour->next;
        }
        for(i = 0 ; i < destIndex+1 ; i++){
            dest = dest->next;
        }
        (dest->data) = (dest->data) + (sour->data);
        if(sour->next != NULL){
            sour->next->prev = tmp;
        }
        tmp->next = sour->next;
        delete sour;
        size -= 1;
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    Node<T>* curr = head;
    int indx;
    if(containsNode(node)){
        indx = getIndex(node);
        for(int i = 0; i<indx+1 ;i++){
            curr = curr->next;
        }
        if(curr->next != NULL){
            curr->next->prev = curr->prev;
        }
        curr->prev->next = curr->next;
        delete curr;
        size--;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    Node<T>* curr = getNode(data);
    while(curr != NULL){
        if(curr->next != NULL){
            curr->next->prev = curr->prev;
        }
        curr->prev->next = curr->next;
        delete curr;
        size--;
        curr = getNode(data);
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    if(index < 0 || index >= size){
        return;
    }
    Node<T>* curr = head;
    for(int i = 0; i<index+1 ;i++){
        curr = curr->next;
    }
    if(curr->next != NULL){
        curr->next->prev = curr->prev;
    }
    curr->prev->next = curr->next;
    delete curr;
    size--;
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    Node<T>* tmp;
    while(head->next != NULL){
        tmp = head->next;
        head->next = head->next->next;
        delete tmp;
    }
    size = 0;
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    while(node)
    {
        std::cout << *node << std::endl;
        node = node->next;
    }

}

#endif //LINKEDLIST_H
#endif //LINKEDLIST_H
