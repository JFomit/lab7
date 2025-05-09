#include <iostream>
#include <string>
#include "tree.h"

int main() {
  using lab::TreeTraversal;

  lab::TreeMap<int, std::string> map;
  map.Insert(10, "10");
  map.Insert(9, "9");
  map.Insert(8, "8");
  map.Insert(7, "7");
  map.Insert(6, "6");
  map.Insert(5, "5");
  map.Insert(4, "4");
  map.Insert(3, "3");
  map.Insert(2, "2");
  map.Insert(1, "1");

  map.Traverse<TreeTraversal::kPreOrder>(
      [](std::string &v) { std::cout << v << '\n'; });
  map.Balance();
  std::cout << '\n';
  map.Traverse<TreeTraversal::kPreOrder>(
      [](std::string &v) { std::cout << v << '\n'; });

  return 0;
}
