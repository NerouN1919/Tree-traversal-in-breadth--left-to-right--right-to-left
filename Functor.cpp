#include <stdexcept>
#include "Functor.h"
void sviridenko::Functor::operator()(std::pair< std::string, std::string >& rhs)
{
  if (elementsNumber_ > 0)
  {
    outString_ += " ";
  }
  keySum_ += std::stoi(rhs.first);
  outString_ += rhs.second;
  ++elementsNumber_;
}
