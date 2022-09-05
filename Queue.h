#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include "List.h"
namespace sviridenko
{
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;
    void push(const T&);
    void drop();
    T peek() const;
    bool isEmpty() const;
    void print(std::ostream&) const;
  private:
    sviridenko::List< T > queue;
  };
}
template < typename T >
void sviridenko::Queue< T >::push(const T& d)
{
  queue.pushBack(d);
}

template < typename T >
T sviridenko::Queue< T >::peek() const
{
  return queue.peekFront();
}

template < typename T >
void sviridenko::Queue< T >::drop()
{
  queue.dropFront();
}

template < typename T >
bool sviridenko::Queue< T >::isEmpty() const
{
  return queue.isEmpty();
}

template < typename T >
void sviridenko::Queue< T >::print(std::ostream& stream) const
{
  queue.print(stream);
}

#endif
