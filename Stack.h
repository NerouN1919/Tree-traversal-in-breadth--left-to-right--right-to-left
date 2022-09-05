#ifndef STACK_H
#define STACK_H
#include <iostream>
#include "List.h"
namespace sviridenko
{
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;
    void push(const T&);
    void drop();
    T peek() const;
    bool isEmpty() const;
    void print(std::ostream&) const;

  private:
    sviridenko::List< T > stack;
    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;
    Stack& operator=(const Stack&) = delete;
    Stack& operator=(Stack&&) = delete;
  };
}
template < typename T >
T sviridenko::Stack< T >::peek() const
{
  return stack.peekFront();
}

template < typename T >
void sviridenko::Stack< T >::push(const T& val)
{
  stack.pushFront(val);
}

template < typename T >
bool sviridenko::Stack< T >::isEmpty() const
{
  return stack.isEmpty();
}

template < typename T >
void sviridenko::Stack< T >::drop()
{
  stack.dropFront();
}

template < typename T >
void sviridenko::Stack< T >::print(std::ostream& stream) const
{
  stack.print(stream);
}
#endif
