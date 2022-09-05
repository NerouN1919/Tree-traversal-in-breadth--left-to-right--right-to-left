#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include <iostream>
#include "Queue.h"
#include "Stack.h"
namespace sviridenko
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree
  {
  public:
    using val = std::pair< Key, Value >;
    BinarySearchTree();
    ~BinarySearchTree();
    BinarySearchTree(const BinarySearchTree&);
    BinarySearchTree(BinarySearchTree&&) noexcept;
    BinarySearchTree& operator=(const BinarySearchTree& other);
    BinarySearchTree& operator= (BinarySearchTree&& src) noexcept;
    struct Node
    {
    public:
      val key_;
      Node* left_;
      Node* right_;
      Node* p_;
      Node(const val& key, Node* left = nullptr, Node* right = nullptr, Node* p = nullptr) :
        key_(key), left_(left), right_(right), p_(p)
      { }
    };
    bool search(const val& key) const;
    void insert(const Key&, const Value&);
    bool isEmpty() const;
    struct Iterator
    {
      Iterator& operator++();
      Iterator operator++(int);
      bool operator!=(const Iterator&) const;
      bool operator==(const Iterator&) const;
      Iterator() = default;
      Iterator(Node*, BinarySearchTree*);
      std::pair< Key, Value >& operator*();
      std::pair< Key, Value >* operator->();
    private:
      Node* current_ = nullptr;
      BinarySearchTree* tree_ = nullptr;
      friend BinarySearchTree< Key, Value, Compare >;
    };
    struct ConstIterator
    {
      ConstIterator& operator++();
      ConstIterator operator++(int);
      bool operator!=(const ConstIterator&) const;
      bool operator==(const ConstIterator&) const;
      ConstIterator() = default;
      ConstIterator(Node*, const BinarySearchTree*);
      std::pair< Key, Value >& operator*() const;
      std::pair< Key, Value >* operator->() const;
    private:
      Node* current_ = nullptr;
      const BinarySearchTree* tree_ = nullptr;
      friend BinarySearchTree< Key, Value, Compare >;
    };
    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;
    ConstIterator cminIt(Node*) const;
    void remove(const Key& item);
    template< typename F >
    F traverseLnr(F f) const;
    template< typename F >
    F traverseRnl(F f) const;
    template< typename F >
    F traverseBreadth(F f) const;
  private:
    Node* root_;
    Compare cmp_;
    Iterator minIt(Node* current);
    Node* iterativeSearchNode(const Key& key) const;
    int getCount(Node* node) const;
    void swap(BinarySearchTree& other);
    void deleteTree(Node* node);
    bool insertNode(const val& key);
    Node* deleteNode(Node*, const Key&);
  };

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::BinarySearchTree() :
    root_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::BinarySearchTree(BinarySearchTree&& src) noexcept :
    root_(src.root_),
    cmp_(src.cmp_)
  {
    src.root_ = nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >&
    sviridenko::BinarySearchTree< Key, Value, Compare >::operator=(const BinarySearchTree& other)
  {
    BinarySearchTree< Key, Value, Compare > newTree(other);
    swap(newTree);
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >&
    sviridenko::BinarySearchTree< Key, Value, Compare >::operator=(BinarySearchTree&& other) noexcept
  {
    if (this != &other)
    {
      swap(other);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::~BinarySearchTree()
  {
    if (root_ != nullptr)
    {
      deleteTree(root_);
    }
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const BinarySearchTree& other) :
    root_(nullptr)
  {
    for (ConstIterator otherIt = other.cbegin(); otherIt != other.cend(); ++otherIt)
    {
      insert(otherIt->first, otherIt->second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::isEmpty() const
  {
    return cbegin() == cend();
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::Iterator(Node* node, BinarySearchTree* tree)
  {
    this->current_ = node;
    this->tree_ = tree;
  }

  template< typename Key, typename Value, typename Compare >
  sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node* node, const BinarySearchTree* tree
  )
  {
    this->current_ = node;
    this->tree_ = tree;
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::begin()
  {
    return minIt(root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::end()
  {
    return Iterator();
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::cbegin() const
  {
    return cminIt(root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::cend() const
  {
    return ConstIterator();
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::cminIt(Node* current) const
  {
    if (current == nullptr)
    {
      return ConstIterator(nullptr, this);
    }
    while (current->left_ != nullptr)
    {
      current = current->left_;
    }
    ConstIterator it(current, this);
    return it;
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator&
    sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator++()
  {
    Node* ptr = nullptr;
    if (current_ == nullptr)
    {
      current_ = tree_->root_;
      assert(current_ != nullptr);

      while (current_->left_ != nullptr)
      {
        current_ = current_->left_;
      }
    }
    else
    {
      if (current_->right_ != nullptr)
      {
        current_ = current_->right_;

        while (current_->left_ != nullptr)
        {
          current_ = current_->left_;
        }
      }
      else
      {
        ptr = current_->p_;
        while (ptr != nullptr && current_ == ptr->right_)
        {
          current_ = ptr;
          ptr = ptr->p_;
        }
        current_ = ptr;
      }
    }

    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator&
    sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++()
  {
    Node* ptr = nullptr;
    if (current_ == nullptr)
    {
      current_ = tree_->root_;

      while (current_->left_ != nullptr)
      {
        current_ = current_->left_;
      }
    }
    else
    {
      if (current_->right_ != nullptr)
      {
        current_ = current_->right_;

        while (current_->left_ != nullptr)
        {
          current_ = current_->left_;
        }
      }
      else
      {
        ptr = current_->p_;
        while (ptr != nullptr && current_ == ptr->right_)
        {
          current_ = ptr;
          ptr = ptr->p_;
        }
        current_ = ptr;
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator++(int)
  {
    Iterator result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator
    sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++(int)
  {
    ConstIterator result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator*()
  {
    assert(this->current_ != nullptr);
    return this->current_->key_;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator->()
  {
    assert(this->current_ != nullptr);
    return &(this->current_->key_);
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator*() const
  {
    return this->current_->key_;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator->() const
  {
    assert(this->current_ != nullptr);
    return &(this->current_->key_);
  }

  template< typename Key, typename Value, typename Compare >
  bool sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator!=(const Iterator& other) const
  {
    return !(this->current_ == other.current_);
  }

  template< typename Key, typename Value, typename Compare >
  bool sviridenko::BinarySearchTree< Key, Value, Compare >::Iterator::operator==(const Iterator& other) const
  {
    return this->current_ == other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  bool sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator!=(const ConstIterator& other) const
  {
    return !(this->current_ == other.current_);
  }

  template< typename Key, typename Value, typename Compare >
  bool sviridenko::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator==(const ConstIterator& other) const
  {
    return this->current_ == other.current_;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::minIt(Node* current)
  {
    Iterator it(cminIt(current).current_, this);
    return it;
  }
  template < typename Key, typename Value, typename Compare >
  int BinarySearchTree< Key, Value, Compare >::getCount(Node* node) const
  {
    if (node == nullptr)
      return 0;
    return (1 + getCount(node->left_) + getCount(node->right_));
  }

  template < typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::swap(BinarySearchTree& other)
  {
    std::swap(root_, other.root_);
  }

  template < typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::deleteTree(Node* node)
  {
    if (node)
    {
      deleteTree(node->left_);
      deleteTree(node->right_);
      delete node;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void sviridenko::BinarySearchTree< Key, Value, Compare >::insert(const Key& key, const Value& value)
  {
    std::pair< Key, Value > pair(key, value);
    Node* yatemp = nullptr;
    Node* temp = root_;
    while (temp)
    {
      if (temp->key_.first == pair.first)
      {
        return;
      }
      yatemp = temp;
      if (cmp_(pair.first, temp->key_.first))
      {
        temp = temp->left_;
      }
      else
      {
        temp = temp->right_;
      }
    }
    Node* lasttemp = new Node(pair, nullptr, nullptr, yatemp);
    if (!yatemp)
    {
      root_ = lasttemp;
      return;
    }
    else if (cmp_(pair.first, yatemp->key_.first))
    {
      yatemp->left_ = lasttemp;
      return;
    }
    else
    {
      yatemp->right_ = lasttemp;
      return;
    }
  }

  template < typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::insertNode(const val& key)
  {
    Node* node = new Node(key);
    Node* current = root_;
    Node* tmp = nullptr;
    if (root_ == nullptr)
    {
      root_ = node;
      return true;
    }
    while (current != nullptr)
    {
      if (cmp_(current->key_.first, node->key_.first))
      {
        tmp = current;
        current = current->right_;
      }
      else if (!cmp_(current->key_.first, node->key_.first))
      {
        tmp = current;
        current = current->left_;
      }
      else
      {
        delete node;
        return false;
      }
    }
    if (!cmp_(node->key_.first, tmp->key_.first))
    {
      tmp->right_ = node;
      tmp->right_->p_ = tmp;
    }
    else
    {
      tmp->left_ = node;
      tmp->left_->p_ = tmp;
    }
    return true;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::remove(const Key& item)
  {
    root_ = deleteNode(root_, item);
  }

  template< typename Key, typename Value, typename Compare >
  typename sviridenko::BinarySearchTree< Key, Value, Compare >::Node*
    sviridenko::BinarySearchTree< Key, Value, Compare >::deleteNode(Node* root, const Key& key)
  {
    if (root == nullptr)
    {
      return root;
    }
    else if (key < root->key_.first)
    {
      root->left_ = deleteNode(root->left_, key);
    }
    else if (key > root->key_.first)
    {
      root->right_ = deleteNode(root->right_, key);
    }
    else
    {
      if (root->left_ == nullptr && root->right_ == nullptr)
      {
        delete root;
        root = nullptr;
      }
      else if (root->left_ == nullptr)
      {
        Node* temp = root;
        root = root->right_;
        delete temp;
      }
      else if (root->right_ == nullptr)
      {
        Node* temp = root;
        root = root->left_;
        delete temp;
      }
      else
      {
        Node* tempMin = root_->right_;
        while (tempMin)
        {
          tempMin = tempMin->left_;
        }
        Node* temp = tempMin;
        root->key_.first = temp->key_.first;
        root->right_ = deleteNode(root->right_, temp->key_.first);
      }
    }
    return root;
  }

  template < typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::search(const val& key) const
  {
    return (iterativeSearchNode(key) != nullptr ? true : false);
  }

  template < typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Node* BinarySearchTree< Key, Value, Compare >::iterativeSearchNode(const Key& key) const
  {
    Node* current = root_;
    while (current != nullptr && current->key_.first != key)
    {
      if (key < current->key_.first)
        current = current->left_;
      else if (key > current->key_.first)
        current = current->right_;
    }
    return current;
  }
  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverseBreadth(F f) const
  {
    Queue< Node* > q;
    typename BinarySearchTree< Key, Value, Compare >::Node* tempNode = root_;
    while (tempNode != nullptr)
    {
      f(tempNode->key_);
      if (tempNode->left_ != nullptr)
      {
        q.push(tempNode->left_);
      }
      if (tempNode->right_ != nullptr)
      {
        q.push(tempNode->right_);
      }
      if (q.isEmpty())
      {
        break;
      }
      else
      {
        tempNode = q.peek();
        q.drop();
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template < typename F >
  F sviridenko::BinarySearchTree< Key, Value, Compare >::traverseLnr(F f) const
  {
    for (ConstIterator it = cbegin(); it != cend(); it++)
    {
      f(*it);
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template < typename F >
  F sviridenko::BinarySearchTree< Key, Value, Compare >::traverseRnl(F f) const
  {
    Stack < ConstIterator > stack;
    for (ConstIterator it = cbegin(); it != cend(); ++it)
    {
      stack.push(it);
    }
    while (!stack.isEmpty())
    {
      f(*stack.peek());
      stack.drop();
    }
    return f;
  }
}
#endif
