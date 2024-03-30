#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"
#include "print_bst.h"

using namespace std;


int main(int argc, char *argv[])
{
    /*
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('g',2));
    bt.insert(std::make_pair('b',3));
    bt.insert(std::make_pair('j',4));
    bt.insert(std::make_pair('k',5));
    bt.insert(std::make_pair('x',6));
    bt.insert(std::make_pair('x',7));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    bt.print();
    cout << "Erasing b" << endl;
    bt.remove('b');
    */

    // AVL Tree Tests
    AVLTree<int,int> at;
    at.insert(std::make_pair(1,1));
    at.insert(std::make_pair(3,3));
    at.insert(std::make_pair(5,5));
    at.insert(std::make_pair(10,10));
    at.insert(std::make_pair(6,6));
    at.insert(std::make_pair(11,11));
    at.insert(std::make_pair(8,8));
    at.insert(std::make_pair(4,4));
    at.insert(std::make_pair(7,7));

    at.print();

    return 0;
}
