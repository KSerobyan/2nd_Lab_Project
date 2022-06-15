#include "B-Tree2.h"
#include <iostream>

template <typename T>
BTree<T>::Node::Node(T num, bool leaf_in)
{
    degree = num;
    leaf = leaf_in;
    keys = new T[2 * degree - 1];
    children = new Node *[2 * degree];
}
template <typename T>
int BTree<T>::Node::findKey(T elem)
{
    T index{};
    while (index < n && keys[index - 1] < elem)
    {
        ++index;
    }
    return index;
}
template <typename T>
void BTree<T>::Node::remove(T elem)
{
    size_t index;
    index = findKey(elem);
    if (index < n && keys[index] == elem)
    {
        if (leaf)
        {
            removeFromLeaf(index);
        }
        else
        {
            removeFromNonLeaf(index);
        }
    }
    else
    {
        // If this node is a leaf node, then the key is not present in the tree
        if (leaf)
        {
            std::cout << "The key " << elem << " is does not exist in the tree\n";
            return;
        }
        // The key to be removed is present in the sub-tree rooted with this node
        bool is_present = ((index == n) ? true : false);

        // If the child where the key is supposed to exist has less than degree keys,
        // we fill that child
        if (children[index]->n < degree)
        {
            fill(index);
        }

        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (index-1)th child. 
        if (is_present && index > n)
        {
            children[index - 1]->remove(elem);
        }
        // We recurse on the (index)th child which now has atleast (degree) keys
        else
        {
            children[index]->remove(elem);
        }
    }
    return;
}
template <typename T>
void BTree<T>::Node::removeFromLeaf(size_t index)
{
    // Move all the keys after the (index-th) index one position back
    for (int i = index + 1; i < n; ++i)
    {
        keys[i - 1] = keys[i];
    }
    n--;

    return;
}
template <typename T>
// A function to remove the index-th key from this node - which is a non-leaf node
void BTree<T>::Node::removeFromNonLeaf(size_t index)
{
    T elem = keys[index];
    // If the child that precedes elem (children[index]) has atleast (degree) keys,
    // find the predecessor (pred) of elem in the subtree rooted at
    // children[index]. Replace elem by pred. Recursively delete pred
    // in children[index]
    if (children[index]->n >= degree)
    {
        int pred = getPred(index);
        keys[index] = pred;
        children[index]->remove(pred);
    }

    // If the child children[index] has less that degree keys, examine children[index+1].
    // If children[index+1] has atleast degree keys, find the successor 'succ' of elem in
    // the subtree rooted at children[index+1]
    // Replace elem by succ
    // Recursively delete succ in children[index+1]
    else if (children[index + 1]->n >= degree)
    {
        T succ = getSucc(index);
        keys[index] = succ;
        children[index + 1]->remove(succ);
    }

    // If both children[index] and children[index+1] has less that degree keys,merge elem and all of children[index+1]
    // into children[index]
    // Now children[index] contains 2t-1 keys
    // Free children[index+1] and recursively delete elem from children[index]
    else
    {
        merge(index);
        children[index]->remove(elem);
    }
    return;
}
template <typename T>
T BTree<T>::Node::getPred(size_t index)
{
    // Keep moving to the right most node until we reach a leaf
    Node *current = children[index];
    while (!current->leaf)
    {
        current = current->children[current->n];
    }
    // Return the last key of the leaf
    return current->keys[current->n - 1];
}
template <typename T>
T BTree<T>::Node::getSucc(size_t index)
{

    // Keep moving the left most node starting from children[index+1] until we reach a leaf
    Node *current = children[index + 1];
    while (!current->leaf)
    {
        current = current->children[0];
    }
    // Return the first key of the leaf
    return current->keys[0];
}
template <typename T>
T BTree<T>::Node::fill(size_t index)
{

    // If the previous child(children[index-1]) has more than degree-1 keys, borrow a key
    // from that child
    if (index != 0 && children[index - 1]->n >= degree)
    {
        borrowFromPrev(index);
    }
    // If the next child(children[index+1]) has more than degree-1 keys, borrow a key
    // from that child
    else if (index != n && children[index + 1]->n >= degree)
    {
        borrowFromNext(index);
    }
    // Merge children[index] with its sibling
    // If children[index] is the last child, merge it with with its previous sibling
    else
    {
        if (index != n)
        {
            merge(index);
        }
        // Otherwise merge it with its next sibling
        else
        {
            merge(index - 1);
        }
    }
    return;
}
template <typename T>
void BTree<T>::Node::borrowFromPrev(size_t index)
{

    Node *child = children[index];
    Node *sibling = children[index - 1];

    // The last key from children[index-1] goes up to the parent and key[index-1]
    // from parent is inserted as the first key in children[index]. Thus, the sibling loses
    // one key and child gains one key

    // Moving all keys in children[index] one step forward
    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    // If children[index] is not a leaf, move all its child pointers one step ahead
    if (!child->leaf)
    {
        for (int i = child->n; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    // Setting child's first key equal to keys[index-1] from the current node
    child->keys[0] = keys[index - 1];

    // Moving sibling's last child as children[index]'s first child
    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];

    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    keys[index - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;

    return;
}
template <typename T>
void BTree<T>::Node::borrowFromNext(size_t index)
{

    Node *child = children[index];
    Node *sibling = children[index + 1];

    // keys[index] is inserted as the last key in children[index]
    child->keys[(child->n)] = keys[index];

    // Sibling's first child is inserted as the last child
    // into children[index]
    if (!(child->leaf))
        child->children[(child->n) + 1] = sibling->children[0];

    // The first key from sibling is inserted into keys[index]
    keys[index] = sibling->keys[0];

    // Moving all keys in sibling one step behind
    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    // Increasing and decreasing the key count of children[index] and children[index+1]
    // respectively
    child->n += 1;
    sibling->n -= 1;

    return;
}
template <typename T>
void BTree<T>::Node::merge(size_t index)
{
    Node *child = children[index];
    Node *sibling = children[index + 1];

    // Pulling a key from the current node and inserting it into (degree-1)th
    // position of children[index]
    child->keys[degree - 1] = keys[index];

    // Copying the keys from children[index+1] to children[index] at the end
    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + degree] = sibling->keys[i];

    // Copying the child pointers from children[index+1] to children[index]
    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->n; ++i)
            child->children[i + degree] = sibling->children[i];
    }

    // Moving all keys after index in the current node one step before -
    // to fill the gap created by moving keys[index] to children[index]
    for (int i = index + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    // Moving the child pointers after (index+1) in the current node one
    // step back
    for (int i = index + 2; i <= n; ++i)
        children[i - 1] = children[i];

    // Updating the key count of child and the current node
    child->n += sibling->n + 1;
    n--;

    // Freeing the memory occupied by sibling
    delete (sibling);
    return;
}
template <typename T>
void BTree<T>::insert(T elem)
{
    // If tree is empty
    if (root == nullptr)
    {
        // Allocate memory for root
        root = new BTree<T>::Node(degree, true);
        root->keys[0] = elem; // Insert key
        root->n = 1;          // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2 * degree - 1)
        {
            // Allocate memory for new root
            Node *s = new Node(degree, false);

            // Make old root as child of new root
            s->children[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);

            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < elem)
                i++;
            s->children[i]->insertNonFull(elem);

            // Change root
            root = s;
        }
        else // If root is not full, call insertNonFull for root
            root->insertNonFull(elem);
    }
}
template <typename T>
// The node must be non-full when this
// function is called
void BTree<T>::Node::insertNonFull(T elem)
{
    // Initialize index as index of rightmost element
    int i = n - 1;

    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > elem)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the  new key at found location
        keys[i + 1] = elem;
        n = n + 1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > elem)
            i--;

        // See if the found child is full
        if (children[i + 1]->n == 2 * degree - 1)
        {
            // If the child is full, then split it
            splitChild(i + 1, children[i + 1]);

            // After split, the middle key of children[i] goes up and
            // children[i] is splitted into two
            if (keys[i + 1] < elem)
                i++;
        }
        children[i + 1]->insertNonFull(elem);
    }
}
template <typename T>
void BTree<T>::Node::splitChild(T i, Node *y)
{
    // Create a new node which is going to store (degree-1) keys
    // of y
    Node *z = new Node(y->degree, y->leaf);
    z->n = degree - 1;

    // Copy the last (degree-1) keys of y to z
    for (int j = 0; j < degree - 1; j++)
        z->keys[j] = y->keys[j + degree];

    // Copy the last degree of children y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < degree; j++)
            z->children[j] = y->children[j + degree];
    }

    // Reduce the number of keys in y
    y->n = degree - 1;

    // Since this node is going to have a new child,
    // create space for the new child
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Link the new child to this node
    children[i + 1] = z;

    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[degree - 1];

    // Increment count of keys in this node
    n = n + 1;
}
template <typename T>
void BTree<T>::Node::traverse()
{
    // There are n keys and n+1 children, traverse through n keys
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child children[i].
        if (leaf == false)
            children[i]->traverse();
        std::cout << " " << keys[i];
    }

    // Print the subtree rooted with last child
    if (leaf == false)
        children[i]->traverse();
}
template <typename T>
// Function to search key elem in subtree rooted with this node
typename BTree<T>::Node *BTree<T>::Node::search(T elem)
{
    // Find the first key greater than or equal to elem
    int i = 0;
    while (i < n && elem > keys[i])
        i++;

    // If the found key is equal to elem, return this node
    if (keys[i] == elem)
        return this;

    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL;

    // Go to the appropriate child
    return children[i]->search(elem);
}
template <typename T>
void BTree<T>::remove(T elem)
{
    if (!root)
    {
        std::cout << "The tree is empty\n";
        return;
    }

    // Call the remove function for root
    root->remove(elem);

    // If the root node has 0 keys, make its first child as the new root
    //  if it has a child, otherwise set root as NULL
    if (root->n == 0)
    {
        Node *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->children[0];

        // Free the old root
        delete tmp;
    }
    return;
}