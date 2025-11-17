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
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);

};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
  if (x == NULL)
  {
    return;
  }
  AVLNode<Key, Value>* y = x->getRight();
  if (y == NULL)
  {
    return;
  }
  AVLNode<Key, Value>* parent = x->getParent();

  x->setRight(y->getLeft());
  if (y->getLeft() != NULL)
  {
    y->getLeft()->setParent(x);
  }
  y->setLeft(x);
  x->setParent(y);
  y->setParent(parent);
  if (parent == NULL)
  {
    // if the parent is NULL that means that y is now the root so update for balance
    this->root_ = y;
  }
  else if (x == parent->getLeft())
  {
    parent->setLeft(y);
  }
  else{
    parent->setRight(y);
  }
  // update the balance using setBalance
  int8_t xbal = x->getBalance();
  int8_t ybal = y->getBalance();

  int8_t xb = xbal - 1 - std::max(0, (int)ybal);
  x->setBalance(xb);

  int8_t yb = ybal - 1 + std::min(0, (int)xbal);
  y->setBalance(yb);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x)
{
  if (x == NULL)
  {
    return;
  }
  AVLNode<Key, Value>* y = x->getLeft();
  if (y == NULL)
  {
    return;
  }
  AVLNode<Key, Value>* parent = x->getParent();

  x->setLeft(y->getRight());
  if (y->getRight() != NULL)
  {
    y->getRight()->setParent(x);
  }
  y->setRight(x);
  x->setParent(y);
  y->setParent(parent);
  if (parent == NULL)
  {
    // if the parent is NULL that means that y is now the root so update for balance
    this->root_ = y;
  }
  else if (x == parent->getLeft())
  {
    parent->setLeft(y);
  }
  else{
    parent->setRight(y);
  }
  // update the balance using setBalance
  int8_t xbal = x->getBalance();
  int8_t ybal = y->getBalance();
  int8_t xb = xbal + 1 - std::min(0, (int)ybal);
  x->setBalance(xb);

  int8_t yb = ybal + 1 + std::max(0, (int)xbal);
  y->setBalance(yb);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
  if (parent == NULL || parent->getParent() == NULL)
  {
    return;
  }
  AVLNode<Key, Value>* g = parent->getParent();
  if (parent == g->getLeft())
  {
    g->updateBalance(-1);
    if (g->getBalance() == -1)
    {
      insertFix(g, parent);
    }
    else if (g->getBalance() == -2)
    {
      if (child == parent->getLeft())
      {
        rotateRight(g);
      }
      else{
        rotateLeft(parent);
        rotateRight(g);
      }
    }
  }
  else{
    g->updateBalance(1);
    if (g->getBalance() == 1)
    {
      insertFix(g, parent);
    }
    else if (g->getBalance() == 2)
    {
      if (child == parent->getRight())
      {
        rotateLeft(g);
      }
      else{
        rotateRight(parent);
        rotateLeft(g);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
  while (node != NULL)
  {
    node->updateBalance(diff);
    if (node->getBalance() == 2)
    {
      AVLNode<Key, Value>* c = node->getRight();
      if (c->getBalance() >= 0)
      {
        rotateLeft(node);
        if (c->getBalance() == 0)
        {
          return;
        }
      }
      else
      {
        rotateRight(c);
        rotateLeft(node);
      }
    }
    else if (node->getBalance() == -2)
    {
      AVLNode<Key, Value>* c = node->getLeft();
      if (c->getBalance() <= 0)
      {
        rotateRight(node);
        if (c->getBalance() == 0)
        {
          return;
        }
      }
      else
      {
        rotateLeft(c);
        rotateRight(node);
      }
    }
    else if (node->getBalance() != 0)
    {
      return;
    }
    AVLNode<Key, Value>* parent = node->getParent();
    if (parent != NULL)
    {
      if (node == parent->getLeft())
      {
        diff = 1;
      }
      else{
        diff = -1;
      }
    }
    node = parent;
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    const Key& key = new_item.first;
    const Value& value = new_item.second;
    if (this->root_ == NULL)
    {
        this->root_ = new AVLNode<Key, Value>(key, value, NULL);
        return;
    }
    AVLNode<Key, Value>* c = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* p = NULL;
    while (c != NULL)
    {
      p = c;
      if (key <c->getKey())
      {
        c = c->getLeft();
      }
      else if (key > c->getKey())
      {
        c = c->getRight();
      }
      else
      {
        c->setValue(value);
        return;
      }
    }
      AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(key, value, p);
      if (key < p->getKey())
          p->setLeft(newNode);
      else
          p->setRight(newNode);

      if (p->getBalance() == 0)
      {
        if (newNode == p->getLeft())
        {
          p->setBalance(-1);
        }
        else
        {
          p->setBalance(1);
        }
        insertFix(p, newNode);
      }

      else
      {
        p->setBalance(0);
        return;
      }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == NULL)
    {
      return;
    }
    if (node->getLeft() != NULL && node->getRight() != NULL)
    {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
      nodeSwap(node, pred);
    }
    AVLNode<Key, Value>* child = (node->getLeft() != NULL ? node->getLeft() : node->getRight());
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t balance = 0;

    if (child != NULL)
    {
      child->setParent(parent);
    }
    if (parent == NULL)
    {
      this->root_ = child;
    }
    else if (node == parent->getLeft())
    {
      parent->setLeft(child);
      balance = 1;
    }
    else{
      parent->setRight(child);
      balance = -1;
    }
    delete node;
    if (parent != NULL)
    {
      int8_t diff = balance;
      removeFix(parent, diff);
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
