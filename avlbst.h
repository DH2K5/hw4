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
    virtual void insertFix(AVLNode<Key, Value>* prev, AVLNode<Key, Value>* curr);
    virtual void removeFix(AVLNode<Key, Value>* curr, int diff);
    virtual void insertLeft(AVLNode<Key, Value>* pare);
    virtual void insertRight(AVLNode<Key, Value>* pare);
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
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*> (this->root_);
    AVLNode<Key, Value>* prev = static_cast<AVLNode<Key, Value>*> (this->root_);
    if(this->root_ == nullptr){
        curr = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_ = curr;
        return;
    }
    while(curr != nullptr){
        prev = curr;
        if(new_item.first == (curr->getKey())){
            curr->setValue(new_item.second);
            return;
        }
        if(new_item.first < (curr->getKey())){
            curr = curr->getLeft();
        }
        if(new_item.first > (curr->getKey())){
            curr = curr->getRight();
        }
    }
    if(new_item.first < (prev->getKey())){
        curr = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
        prev->setLeft(curr);
        prev->updateBalance(-1);
    }
    else if(new_item.first > (prev->getKey())){
        curr = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
        prev->setRight(curr);
        prev->updateBalance(1);
    }
    if((prev->getBalance()) == 0){
        return;
    }
    insertFix(prev, curr);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* prev,  AVLNode<Key, Value>* curr){
    if((prev == nullptr) || (curr == nullptr) || ((prev->getParent()) == nullptr)){
        return;
    }
    if((prev->getBalance() >= -1) && (prev->getBalance() <= 1) && (curr->getBalance() >= -1) && (curr->getBalance() <= 1)){
        return;
    }
    AVLNode<Key, Value>* pare = (prev->getParent());
    if((pare->getLeft()) == prev){
        pare->updateBalance(-1);
        if((pare->getBalance()) == 0){
            return;
        }
        else if((pare->getBalance()) == -1){
            insertFix(pare, prev);
        }
        else if((pare->getBalance()) == -2){
            if(prev->getBalance() == -1){
                insertLeft(pare);
                prev->setBalance(0);
                pare->setBalance(0);
            }
            else if(prev->getBalance() == 1){
                insertRight(prev);
                insertLeft(pare);
                if((curr->getBalance()) == -1){
                    prev->setBalance(0);
                    pare->setBalance(+1);
                    curr->setBalance(0);
                }
                else if((curr->getBalance()) == 0){
                    prev->setBalance(0);
                    pare->setBalance(0);
                    curr->setBalance(0);
                }
                else if((curr->getBalance()) == 1){
                    prev->setBalance(+1);
                    pare->setBalance(0);
                    curr->setBalance(0);
                }
            }
            return;
        }
    }
    else if((pare->getRight()) == prev){
        pare->updateBalance(1);
        if((pare->getBalance()) == 0){
            return;
        }
        else if((pare->getBalance()) == 1){
            insertFix(pare, prev);
        }
        else if((pare->getBalance()) == 2){
            if(prev->getBalance() == 1){
                insertRight(pare);
                prev->setBalance(0);
                pare->setBalance(0);
            }
            else if(prev->getBalance() == -1){
                insertLeft(prev);
                insertRight(pare);
                if((curr->getBalance()) == 1){
                    prev->setBalance(0);
                    pare->setBalance(-1);
                    curr->setBalance(0);
                }
                else if((curr->getBalance()) == 0){
                    prev->setBalance(0);
                    pare->setBalance(0);
                    curr->setBalance(0);
                }
                else if((curr->getBalance()) == -1){
                    prev->setBalance(-1);
                    pare->setBalance(0);
                    curr->setBalance(0);
                }
            }
            return;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertLeft(AVLNode<Key, Value>* pare){
    AVLNode<Key, Value>* prev = (pare->getLeft());
    AVLNode<Key, Value>* temp = (prev->getRight());
    if((pare == nullptr) || (prev == nullptr)){
        return;
    }
    prev->setRight(pare);
    if((pare->getParent()) == nullptr){
        this->root_ = prev;
    }
    else if(((pare->getParent())->getLeft()) == pare){
        (pare->getParent())->setLeft(prev);
    }
    else if(((pare->getParent())->getRight()) == pare){
        (pare->getParent())->setRight(prev);
    }
    pare->setParent(prev);
    pare->setLeft(temp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertRight(AVLNode<Key, Value>* pare){
    AVLNode<Key, Value>* prev = (pare->getRight());
    AVLNode<Key, Value>* temp = (prev->getLeft());
    if((pare == nullptr) || (prev == nullptr)){
        return;
    }
    prev->setLeft(pare);
    if((pare->getParent()) == nullptr){
        this->root_ = prev;
    }
    else if(((pare->getParent())->getLeft()) == pare){
        (pare->getParent())->setLeft(prev);
    }
    else if(((pare->getParent())->getRight()) == pare){
        (pare->getParent())->setRight(prev);
    }
    pare->setParent(prev);
    pare->setRight(temp);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*> (this->root_);
    while(curr != nullptr){
        if(key == (curr->getKey())){
            if(((curr->getLeft()) != nullptr) && ((curr->getRight()) != nullptr)){
                nodeSwap(this->predecessor(curr), curr);
                return remove(curr->getKey());
            }
            AVLNode<Key, Value>* prev = curr->getParent();
            int diff = 0;
            if(prev != nullptr){
                if((prev->getLeft()) == curr){
                    diff = 1;
                    prev->setLeft(nullptr);
                    if((curr->getLeft()) != nullptr){
                        prev->setLeft(curr->getLeft());
                        (curr->getLeft())->setParent(prev);
                    }
                    else if((curr->getRight()) != nullptr){
                        prev->setLeft(curr->getRight());
                        (curr->getRight())->setParent(prev);
                    }
                }
                else if((prev->getRight()) == curr){
                    diff = -1;
                    prev->setRight(nullptr);
                    if((curr->getLeft()) != nullptr){
                        prev->setRight(curr->getLeft());
                        (curr->getLeft())->setParent(prev);
                    }
                    else if((curr->getRight()) != nullptr){
                        prev->setRight(curr->getRight());
                        (curr->getRight())->setParent(prev);
                    }
                }
                removeFix(prev, diff);
            }
            else{
                if((curr->getLeft()) != nullptr){
                    this->root_ = curr->getLeft();
                    (curr->getLeft())->setParent(nullptr);
                }
                else if((curr->getRight()) != nullptr){
                    this->root_ = curr->getRight();
                    (curr->getRight())->setParent(nullptr);
                }
            }
            delete curr;
            return;
        }
        else if(key < (curr->getKey())){
            curr = curr->getLeft();
        }
        else if(key > (curr->getKey())){
            curr = curr->getRight();
        }
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
    AVLNode<Key, Value>* prev = (curr->getParent());
    if(curr == nullptr){
        while(prev != nullptr){
            if(curr == (prev->getRight())){
                return curr;
            }
            curr = prev;
            prev = curr->getParent();
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
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* curr, int diff){
    if(curr == nullptr){
        return;
    }
    AVLNode<Key, Value>* prev = curr->getParent();
    AVLNode<Key, Value>* chil; 
    AVLNode<Key, Value>* gchi;
    if(prev == nullptr){
        return;
    }
    if((prev->getRight()) == curr){
        if(((curr->getBalance()) + diff) == -2){
            chil = (curr->getLeft());
            if((chil->getBalance()) == -1){
                insertLeft(curr);
                curr->setBalance(0);
                chil->setBalance(0);
                return removeFix(curr, -1);
            }
            else if((chil->getBalance()) == 0){
                insertLeft(curr);
                curr->setBalance(-1);
                chil->setBalance(+1);
                return;
            }
            else if((chil->getBalance()) == 1){
                gchi = chil->getRight();
                insertRight(chil);
                insertLeft(curr);
                if((gchi->getBalance()) == 1){
                    curr->setBalance(0);
                    chil->setBalance(-1);
                    gchi->setBalance(0);
                }
                else if((gchi->getBalance()) == 0){
                    curr->setBalance(0);
                    chil->setBalance(0);
                    gchi->setBalance(0);
                }
                else if((gchi->getBalance()) == -1){
                    curr->setBalance(+1);
                    chil->setBalance(0);
                    gchi->setBalance(0);
                }
                return removeFix(curr, -1);
            }
        }
        else if(((curr->getBalance()) + diff) == -1){
            curr->setBalance(-1);
            return;
        }
        else if(((curr->getBalance()) + diff) == 0){
            curr->setBalance(0);
            return removeFix(curr, -1);
        }
    }
    else if((prev->getLeft()) == curr){
        if(((curr->getBalance()) + diff) == 2){
            chil = (curr->getRight());
            if((chil->getBalance()) == 1){
                insertRight(curr);
                curr->setBalance(0);
                chil->setBalance(0);
                return removeFix(curr, 1);
            }
            else if((chil->getBalance()) == 0){
                insertRight(curr);
                curr->setBalance(1);
                chil->setBalance(-1);
                return;
            }
            else if((chil->getBalance()) == -1){
                gchi = chil->getLeft();
                insertLeft(chil);
                insertRight(curr);
                if((gchi->getBalance()) == -1){
                    curr->setBalance(0);
                    chil->setBalance(1);
                    gchi->setBalance(0);
                }
                else if((gchi->getBalance()) == 0){
                    curr->setBalance(0);
                    chil->setBalance(0);
                    gchi->setBalance(0);
                }
                else if((gchi->getBalance()) == 1){
                    curr->setBalance(-1);
                    chil->setBalance(0);
                    gchi->setBalance(0);
                }
                return removeFix(curr, 1);
            }
        }
        else if(((curr->getBalance()) + diff) == 1){
            curr->setBalance(1);
            return;
        }
        else if(((curr->getBalance()) + diff) == 0){
            curr->setBalance(0);
            return removeFix(curr, 1);
        }
    }
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
