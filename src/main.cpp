#include <string>
#include "tree.h"

int main() {
  lab::TreeMap<int, std::string> map;
  map.Insert(5, "hi");
  map.Emplace(0, "hello");
  map.Insert(8, "basic_string<char> &&value");
  map.PreOrderTraversal([](int key, std::string &v) {});
  return 0;
}
