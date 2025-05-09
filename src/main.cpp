#include <iostream>
#include <string>
#include "tree.h"

class MyTree : public lab::TreeMap<int, std::string> {
 public:
  void PrintLeft() {
    TraverseNodes<lab::TreeTraversal::kInOrder>(
        [](const Node *const node) { std::cout << node->data << '\n'; },
        Root());
  }

  [[nodiscard]] int Key() const { return Root()->key; }
};

int main() {
  using lab::TreeTraversal;

  MyTree map;
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

  map.Balance();
  std::cout << map.Key() << '\n';
  map.EraseTree(7);
  std::cout << map.Count() << '\n';
  return 0;
}
