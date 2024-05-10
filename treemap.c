#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
       tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current =NULL;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) return;
    if (searchTreeMap(tree, key) != NULL) return;       
    TreeNode * newNode = createTreeNode(key, value);
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        } 
    else {
        TreeNode * current = tree->root;
        TreeNode * parent = NULL;
        while (current != NULL) {
            parent = current;
            if (tree->lower_than(key, current->pair->key)) {
                current = current->left;
                } 
            else {
                current = current->right;
                }
        }
        newNode->parent = parent;
        if (tree->lower_than(key, parent->pair->key)) {
           parent->left = newNode;
            } 
        else {
                parent->right = newNode;
            }
    
        tree->current = newNode;
        }
    
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode * y = NULL;
    if (node->left == NULL || node->right == NULL) {
        y = node;
    } else {
        y = minimum(node->right);
    }

    TreeNode * x = NULL;
    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != NULL) {
        x->parent = y->parent;
    }

    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != node) {
        node->pair = y->pair;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * node = tree->root;
    while (node != NULL)
        {
            if (is_equal(tree,node->pair->key,key)){
                tree->current = node;
                return node->pair;
            }
            if (tree->lower_than(node->pair->key,key))
            {
                node = node->right;   
            }
            else 
            {
                node = node->left;
            }
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;
    TreeNode * ub_node = NULL;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            ub_node = current;
            current = current->left;
        } else if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        } else {
            current = current->right;
        }
    }

    if (ub_node != NULL) {
        tree->current = ub_node;
        return ub_node->pair;
    }

    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode * current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }

    tree->current = current;
    return current->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * current = tree->current;
    TreeNode * successor = NULL;

    if (current->right != NULL) {
        successor = minimum(current->right);
    } else {
        TreeNode * parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        successor = parent;
    }

    if (successor != NULL) {
        tree->current = successor;
        return successor->pair;
    }

    return NULL;
}
