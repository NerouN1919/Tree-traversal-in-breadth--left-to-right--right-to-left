#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <utility>
#include <string>
#include <cstddef>
namespace sviridenko
{
  struct Functor
  {
    size_t elementsNumber_;
    int keySum_;
    std::string outString_;
    void operator()(std::pair< std::string, std::string >& rhs);
  };
}
#endif
