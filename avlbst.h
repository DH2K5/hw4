#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key, Value>* curr);
    virtual void removeFix(AVLNode<Key, Value>* curr);
    virtual void insertLeft(AVLNode<Key, Value>* pare);
    virtual void insertRight(AVLNode<Key, Value>* pare);
    virtual int maxHeight(AVLNode<Key, Value>* curr);
    virtual AVLNode<Key, Value>* internalFindAVL(const Key& key) const;
    virtual AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    if((this->root_) == nullptr){
        (this->root_) = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    AVLNode<Key, Value>* curr = internalFindAVL(new_item.first);
    if(curr != nullptr){
        curr->setValue(new_item.second);
        return;
    }
    curr = static_cast<AVLNode<Key, Value>*> (this->root_);
    AVLNode<Key, Value>* prev = curr;
    while(curr != nullptr){
        prev = curr;
        if(new_item.first < (curr->getKey())){
            curr->updateBalance(-1);
            curr = curr->getLeft();
        }
        else{
            curr->updateBalance(1);
            curr = curr->getRight();
        }
    }
    curr = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
    if(new_item.first < (prev->getKey())){
        prev->setLeft(curr);
    }
    else{
        prev->setRight(curr);
    }
    while(prev != nullptr){
        prev->setBalance(maxHeight(prev->getRight()) - maxHeight(prev->getLeft()));
        if(((prev->getBalance()) < -1) || ((prev->getBalance()) > 1)){
            insertFix(prev);
        }
        prev = (prev->getParent());
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFindAVL(const Key& key) const{
    if((this->root_) == nullptr){
        return nullptr;
    }
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*> (this->root_);
    while(curr != nullptr){
        if(key == (curr->getKey())){
            break;
        }
        else if(key < (curr->getKey())){
            curr = curr->getLeft();
        }
        else{
            curr = curr->getRight();
        }
    }
    return curr;
}

template<class Key, class Value>
int AVLTree<Key, Value>::maxHeight(AVLNode<Key, Value>* curr){
    if(curr == nullptr){
        return 0;
    }
    int left = maxHeight(curr->getLeft());
    int right = maxHeight(curr->getRight());
    if(left > right){
        return ++left;
    }
    return ++right;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* prev){
    if(prev == nullptr){
        return;
    }
    AVLNode<Key, Value>* curr = prev;
    AVLNode<Key, Value>* next = prev;
    if((prev->getBalance()) < -1){
        if(((prev->getLeft())->getBalance()) < 0){
            curr = (prev->getLeft());
            insertLeft(prev);
        }
        else{
            next = (prev->getLeft());
            insertRight(prev->getLeft());
            curr = (prev->getLeft());
            insertLeft(prev);
        }
    }
    else if((prev->getBalance()) > 1){
        if(((prev->getRight())->getBalance()) > 0){
            curr = (prev->getRight());
            insertRight(prev);
        }
        else{
            next = (prev->getRight());
            insertLeft(prev->getRight());
            curr = (prev->getRight());
            insertRight(prev);
        }
    }
    prev->setBalance(maxHeight(prev->getRight()) - maxHeight(prev->getLeft()));
    curr->setBalance(maxHeight(curr->getRight()) - maxHeight(curr->getLeft()));
    next->setBalance(maxHeight(next->getRight()) - maxHeight(next->getLeft()));
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertLeft(AVLNode<Key, Value>* pare){
    if(pare == nullptr){
        return;
    }
    AVLNode<Key, Value>* prev = (pare->getLeft());
    if(prev == nullptr){
        return;
    }
    AVLNode<Key, Value>* temp = (prev->getRight());
    if((pare->getParent()) == nullptr){
        this->root_ = prev;
        prev->setParent(nullptr);
    }
    else if(((pare->getParent())->getLeft()) == pare){
        (pare->getParent())->setLeft(prev);
        prev->setParent(pare->getParent());
    }
    else if(((pare->getParent())->getRight()) == pare){
        (pare->getParent())->setRight(prev);
        prev->setParent(pare->getParent());
    }
    prev->setRight(pare);
    if(temp != nullptr){
        temp->setParent(pare);
        pare->setLeft(temp);
    }
    else{
        pare->setLeft(nullptr);
    }
    pare->setParent(prev);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertRight(AVLNode<Key, Value>* pare){
    if(pare == nullptr){
        return;
    }
    AVLNode<Key, Value>* prev = (pare->getRight());
    if(prev == nullptr){
        return;
    }
    AVLNode<Key, Value>* temp = (prev->getLeft());
    if((pare->getParent()) == nullptr){
        this->root_ = prev;
        prev->setParent(nullptr);
    }
    else if(((pare->getParent())->getLeft()) == pare){
        (pare->getParent())->setLeft(prev);
        prev->setParent(pare->getParent());
    }
    else if(((pare->getParent())->getRight()) == pare){
        (pare->getParent())->setRight(prev);
        prev->setParent(pare->getParent());
    }
    prev->setLeft(pare);
    if(temp != nullptr){
        temp->setParent(pare);
        pare->setRight(temp);
    }
    else{
        pare->setRight(nullptr);
    }
    pare->setParent(prev);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* curr = internalFindAVL(key);
    if(curr == nullptr){
        return;
    }
    AVLNode<Key, Value>* next = curr;
    if(((curr->getLeft()) != nullptr) && ((curr->getRight()) != nullptr)){
        next = predecessor(curr);
        nodeSwap(curr, next);
        next->setBalance(maxHeight(next->getRight()) - maxHeight(next->getLeft()));
        curr->setBalance(maxHeight(curr->getRight()) - maxHeight(curr->getLeft()));
        return remove(key);
    }
    if(((curr->getLeft()) == nullptr) && ((curr->getRight()) == nullptr)){
        next = (curr->getParent());
        if(next == nullptr){
            (this->root_) = nullptr;
            delete curr;
            return;
        }
        else if((next->getRight()) == curr){
            next->setRight(nullptr);
        }
        else{
             next->setLeft(nullptr);
        }
        delete curr;
        curr = next;
        while(curr != nullptr){
            curr->setBalance(maxHeight(curr->getRight()) - maxHeight(curr->getLeft()));
            if(((curr->getBalance()) < -1) || ((curr->getBalance()) > 1)){
                insertFix(curr);
            }
            curr = (curr->getParent());
        }
        return;
    }
    if((curr->getLeft()) != nullptr){
        next = curr->getLeft();
        nodeSwap(next, curr);
        next->setLeft(nullptr);
    }
    else if((curr->getRight()) != nullptr){
        next = curr->getRight();
        nodeSwap(next, curr);
        next->setRight(nullptr);
    }
    delete curr;
    curr = next;
    while(curr != nullptr){
        curr->setBalance(maxHeight(curr->getRight()) - maxHeight(curr->getLeft()));
        if(((curr->getBalance()) < -1) || ((curr->getBalance()) > 1)){
            insertFix(curr);
        }
        curr = (curr->getParent());
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
    // TODO
    if(current == nullptr){
        return nullptr;
    }
    AVLNode<Key, Value>* curr = (current->getLeft());
    AVLNode<Key, Value>* prev = (current->getParent());
    if(curr == nullptr){
        curr = prev;
        while(prev != nullptr){
            if(curr == (prev->getRight())){
                return prev;
            }
            curr = prev;
            prev = (prev->getParent());
        }
        return nullptr;
    }
    prev = curr;
    while(curr != nullptr){
        prev = curr;
        curr = curr->getRight();
    }
    return prev;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* curr){
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
