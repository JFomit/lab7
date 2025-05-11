#include <cassert>
#include <iostream>
#include <string>
#include "hashtable.h"
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
  lab::HashTable<int, std::string> m;
  m.Insert(5, "basic_string<char> &&value");
  m.Insert(4, "bfe");
  assert(m.Remove(3) == false);
  assert(m.Remove(5) == true);
  return 0;
}
