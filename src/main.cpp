#include <cassert>
#include <iostream>
#include <string>
#include "list.h"
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

struct NoDefault {
  NoDefault() = delete;
  NoDefault(int x) : x_(x) {}
  int x_;
};

int main() {
  lab::List<int> strings;
  strings.emplace_back(5);
  strings.emplace_front(-2);
  strings.emplace_front(3);
  strings.emplace_back(17);

  auto *p = strings.max();
  std::cout << p->item << '\n';
  return 0;
}
