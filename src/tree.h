#ifndef LAB_TREE_H_
#define LAB_TREE_H_

#include <cassert>
// #include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace lab {
template <typename Key, typename Value>
// requires(std::totally_ordered<Key> && std::copyable<Key>)
class TreeMap {
 public:
  TreeMap() : root_(nullptr) {}
  ~TreeMap() { delete root_; }

  void Insert(const Key &key, Value &&value) {
    if (count_ == 0) {
      root_ = new Node(key, value);
      count_ = 1;
    } else {
      InsertInternal(key, std::forward<Value>(value));
    }
  }
  template <typename... Args>
  void Emplace(const Key &key, Args &&...args) {
    if (count_ == 0) {
      root_ = new Node(key, std::forward<Args>(args)...);
      count_ = 1;
    } else {
      EmplaceInternal(key, std::forward<Args>(args)...);
    }
  }

  template <typename F, typename T, typename Aggregator>
  T PreOrderTraversal(F &&func) {
    if constexpr (std::is_invocable_v<F, Value &>) {
    } else if constexpr (std::is_invocable_v<F, const Key &, Value &>) {
    } else {
      static_assert(false,
                    "func must be T(Value &), or T(const Key &, Value &).");
    }
  }

 private:
  struct Node {
    Node(const Key &key, Value &&data, Node *parent = nullptr)
        : key(key), data(data), left(nullptr), right(nullptr), parent(parent) {}
    template <typename... Args>
    Node(const Key &key, Args &&...args, Node *parent = nullptr)
        : key(key),
          data(std::forward<Args>(args)...),
          left(nullptr),
          right(nullptr),
          parent(parent) {}
    ~Node() {
      delete left;
      delete right;
    }

    Key key;
    Value data;

    Node *left;
    Node *right;
    Node *parent;  // for iterator support
  };

  Node *root_;
  size_t count_{};

  void InsertInternal(const Key &key, Value &&value) {
    auto current = root_;
    while (true) {
      if (key > current->key) {
        // right subtree
        if (current->right == nullptr) {
          current->right = new Node(key, value, current);
          break;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          current->left = new Node(key, value, current);
          break;
        }
        current = current->left;
      } else /* if (key == current.key) */ {
        // current node
        current->data = value;
        break;
      }
    }

    ++count_;
  }
  template <typename... Args>
  void EmplaceInternal(const Key &key, Args &&...args) {
    auto current = root_;
    while (true) {
      if (key > current->key) {
        // right subtree
        if (current->right == nullptr) {
          current->right = new Node(key, std::forward<Args>(args)..., current);
          break;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          current->left = new Node(key, std::forward<Args>(args)..., current);
          break;
        }
        current = current->left;
      } else /* if (key == current.key) */ {
        // current node
        current->data.~Value();
        new (&current->data) Value(std::forward<Args>(args)...);
        break;
      }
    }

    ++count_;
  }
};
}  // namespace lab

#endif  // LAB_TREE_H_
