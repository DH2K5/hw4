#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node*, int);

bool equalPaths(Node * root)
{
    // Add your code below
    if((root == nullptr) || ((root->left) == nullptr) && ((root->right) == nullptr)){
        return true;
    }
    bool flag = true;
    if((root->left != nullptr) && (root->right != nullptr)){
        flag = (flag && (height(root->left, 0) == height(root->right, 0)));
    }
    if(root->left != nullptr){
        flag = (flag && equalPaths(root->left));
    }
    if(root->right != nullptr){
        flag = (flag && equalPaths(root->right));
    }
    return flag;
}

int height(Node* root, int count){
	if(root == nullptr){
		return count;
	}
    count++;
    int count1 = height(root->left, count);
    int count2 = height(root->right, count);
    if(count1 > count2){
        return count1;
    }
    return count2;
}