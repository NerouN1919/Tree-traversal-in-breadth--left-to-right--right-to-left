#ifndef LIST_H
#define LIST_H
#include <iostream>
#include "Node_t.h"
namespace sviridenko
{
  template < typename T >
  class List
  {
  public:
    List();
    List(const List& List);
    ~List();
    List<T>& operator=(const List& list);
    void pushFront(const T&);
    void pushBack(const T&);
    void dropFront();
    T peekFront() const;
    bool isEmpty() const;
    void swap(List&) noexcept;
    void print(std::ostream&) const;
  private:
    node_t< T >* head_;
    node_t< T >* tail_;
  };
}

template < typename T >
sviridenko::List< T >::List() :
  head_(nullptr),
  tail_(nullptr)
{}

template < typename T >
sviridenko::List< T >::List(const List& list) :
  head_(nullptr),
  tail_(nullptr)
{
  List temp;
  node_t< T >* src = list.head_;
  while (src)
  {
    temp.pushBack(src->data);
    src = src->next;
  }
  swap(temp);
}

template <class T>
sviridenko::List<T>& sviridenko::List<T>::operator=(const List& list)
{
  if (this == &list)
  {
    return *this;
  }

  List<T> tempList(list);
  swap(tempList);

  return *this;
}

template < typename T >
void sviridenko::List< T >::swap(List& list) noexcept
{
  std::swap(head_, list.head_);
  std::swap(tail_, list.tail_);
}

template < typename T >
sviridenko::List< T >::~List()
{
  while (head_)
  {
    node_t< T >* temp = head_;
    head_ = head_->next;
    delete temp;
  }
}

template < typename T >
void sviridenko::List< T >::pushBack(const T& d)
{
  if (!head_)
  {
    head_ = tail_ = new node_t< T >;
  }
  else
  {
    tail_->next = new node_t< T >;
    tail_ = tail_->next;
  }
  tail_->data = d;
  tail_->next = nullptr;
}

template < typename T >
void sviridenko::List< T >::pushFront(const T& d)
{
  node_t< T >* newNode = new node_t< T >;
  newNode->data = d;
  newNode->next = head_;
  head_ = newNode;
}

template < typename T >
T sviridenko::List< T >::peekFront() const
{
  node_t< T >* temp = head_;
  return temp->data;
}

template < typename T >
void sviridenko::List< T >::dropFront()
{
  if (!head_)
  {
    throw std::logic_error("Standque is empty");
  }
  node_t< T >* temp = head_;
  if (head_ == tail_)
  {
    head_ = tail_ = nullptr;
  }
  else
  {
    head_ = head_->next;
  }
  delete temp;
}

template < typename T >
bool sviridenko::List< T >::isEmpty() const
{
  if (!head_)
  {
    return true;
  }
  return false;
}

template < typename T >
void sviridenko::List< T >::print(std::ostream& stream) const
{
  node_t< T >* temp = head_;
  while (temp)
  {
    if (temp->next == nullptr)
    {
      stream << temp->data;
    }
    else
    {
      stream << temp->data << " ";
    }
    temp = temp->next;
  }
}

#endif
