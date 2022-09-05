#ifndef NODE_T_H
#define NODE_T_H
namespace sviridenko
{
  template < typename T >
  struct node_t
  {
  public:
    T data;
    node_t* next = nullptr;
  };
}
#endif
