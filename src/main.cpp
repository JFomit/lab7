#include <iostream>
#include <string>
#include "tree.h"

int main() {
  using lab::TreeTraversal;

  lab::TreeMap<int, std::string> map;
  map.Insert(5, "hi");
  map.Emplace(0, "hello");
  map.Insert(8, "basic_string<char> &&value");
  map.Traverse<TreeTraversal::kPreOrder>(
      [](std::string &v) { std::cout << v << '\n'; });
  std::string result{};
  auto r = map.Fold(&result, [](std::string *p, std::string &v) {
    *p = p->append(v);
    return p;
  });
  std::cout << *r << '\n';

  auto k_map = map;

  return 0;
}
