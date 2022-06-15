#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <vector>

template <typename T>
class BTree
{
public:
    class Node
    {
        T *keys;         // An array of keys
        T degree;        // Minimum degree
        Node **children; // An array of child pointers
        size_t n;        // Current number of keys
        bool leaf;       // Is true when node is leaf. Otherwise false

    public:
        Node(T num, bool check_leaf); // Constructor

        // Traversal function
        void traverse();

        // A search function
        Node *search(T elem);

        // Returns the index of the first key that is greater or equal to elem
        int findKey(T elem);

        /*A function to insert a new key in the subtree, the node must be non-full when this
        function is called*/
        void insertNonFull(T elem);

        // A  function to split the child this node.
        void splitChild(T index, Node *child);

        void remove(T elem);

        void removeFromLeaf(size_t index);

        void removeFromNonLeaf(size_t index);

        // A function to get the predecessor in the index-th position
        T getPred(size_t index);

        // A function to get the successor
        T getSucc(size_t index);

        // A function to fill up the child node present in the pos-th position
        T fill(size_t index);

        // A function to borrow a key from the previous node to place in the node of the pos position
        void borrowFromPrev(size_t index);

        // Same thing the other way around
        void borrowFromNext(size_t index);

        // A function to merge pos-th child in the node with the next one(pos + 1)
        void merge(size_t index);
    };

private:
    Node *root;
    size_t degree;

public:
    // Constructor
    BTree(size_t _t)
    {
        root = NULL;
        degree = _t;
    }

    void traverse()
    {
        if (root != NULL)
        {
            root->traverse();
        }
    }

    Node *search(T elem)
    {
        return (root == NULL) ? NULL : root->search(elem);
    }

    void insert(T elem);

    void remove(T elem);
};

#endif