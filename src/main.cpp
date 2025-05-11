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
  lab::List<int> to_insert;
  to_insert.push_back(1);
  to_insert.push_back(2);
  to_insert.push_back(3);
  to_insert.push_back(4);

  lab::List<int> strings;
  strings.push_back(5);
  strings.push_front(-2);
  strings.push_front(3);
  strings.push_front(17);

  auto *min = strings.min();
  auto *max = strings.max();

  strings.for_each([](auto x) { std::cout << x << ' '; });
  std::cout << '\n';
  strings.replace_range(max, max, to_insert);
  strings.for_each([](auto x) { std::cout << x << ' '; });
  std::cout << '\n';
  return 0;
}
