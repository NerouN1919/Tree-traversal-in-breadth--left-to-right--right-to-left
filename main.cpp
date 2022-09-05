#include <iostream>
#include <fstream>
#include "BinarySearchTree.h"
#include "Functor.h"
int main(int argc, char* argv[])
{
  std::ifstream in;
  if (argc == 3)
  {
    std::string filename = argv[2];
    in.open(filename);
  }
  else
  {
    std::cerr << "Must be another value of argumets\n";
    return 1;
  }
  std::string command = argv[1];
  if (command != "descending" && command != "ascending" && command != "breadth")
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  std::string line = "";
  std::getline(in, line);
  sviridenko::List< std::string > words;
  if (line == "")
  {
    std::cout << "<EMPTY>\n";
  }
  else
  {
    if (line != "\0")
    {
      line += ' ';
      std::string temp = "";
      for (size_t i = 0; i < line.size(); i++)
      {
        if (line[i] == ' ')
        {
          words.pushBack(temp);
          temp = "";
        }
        else
        {
          temp += line[i];
        }
      }
    }
    sviridenko::BinarySearchTree< std::string, std::string> tree;
    while (!words.isEmpty())
    {
      std::string key = words.peekFront();
      words.dropFront();
      std::string value = words.peekFront();
      words.dropFront();
      tree.insert(key, value);
    }
    sviridenko::Functor func{ 0, 0 };
    try
    {
      if (command == "descending")
      {
        func = tree.traverseRnl(sviridenko::Functor());
      }
      else if (command == "ascending")
      {
        func = tree.traverseLnr(sviridenko::Functor());
      }
      else if (command == "breadth")
      {
        func = tree.traverseBreadth(sviridenko::Functor());
      }
      std::cout << func.keySum_ << " " << func.outString_ << "\n";
    }
    catch (const std::exception& err)
    {
      std::cerr << err.what();
      return 1;
    }
  }
}
