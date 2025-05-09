#ifndef LAB_TREE_H_
#define LAB_TREE_H_

#include <cassert>
// #include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include "optional_ref.h"

namespace lab {

enum class TreeTraversal : int8_t {
  kPreOrder = 0,
  kInOrder = 1,
  kPostOrder = 2
};

template <typename Key, typename Value>
// requires(std::totally_ordered<Key> && std::copyable<Key>)
class TreeMap {
 public:
  TreeMap() : root_(nullptr) {}
  ~TreeMap() { delete root_; }

  TreeMap(const TreeMap<Key, Value> &other)
      : root_(other.root_->copy()), count_(other.count_) {}
  TreeMap<Key, Value> &operator=(const TreeMap<Key, Value> &rhs) {
    if (this == &rhs) {
      return *this;
    }
    delete root_;
    root_ = rhs.root_->copy();
    count_ = rhs.count_;

    return *this;
  }

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

  template <TreeTraversal Traversal, typename F>
  void Traverse(F &&func) {
    TraverseInternal<Traversal>(std::forward<F>(func), root_);
  }

  template <TreeTraversal Traversal = TreeTraversal::kPostOrder, typename T,
            typename F>
  T Fold(T seed, F &&func) {
    return FoldInternal<Traversal>(std::forward<F>(func), seed, root_);
  }

  template <typename F>
  void Traverse(TreeTraversal traversal, F &&func) {
    switch (traversal) {
      case TreeTraversal::kPreOrder:
        Traverse<TreeTraversal::kPreOrder>(std::forward<F>(func));
        break;
      case TreeTraversal::kInOrder:
        Traverse<TreeTraversal::kInOrder>(std::forward<F>(func));
        break;
      case TreeTraversal::kPostOrder:
        Traverse<TreeTraversal::kPostOrder>(std::forward<F>(func));
        break;
      default:
        assert(false && "Invalid traversal order.");
    }
  }

  template <typename T, typename F>
  T Fold(TreeTraversal traversal, T seed, F &&func) {
    switch (traversal) {
      case TreeTraversal::kPreOrder:
        return Fold<TreeTraversal::kPreOrder>(seed, std::forward<F>(func));
      case TreeTraversal::kInOrder:
        return Fold<TreeTraversal::kInOrder>(seed, std::forward<F>(func));
      case TreeTraversal::kPostOrder:
        return Fold<TreeTraversal::kPostOrder>(seed, std::forward<F>(func));
      default:
        assert(false && "Invalid traversal order.");
    }
  }

  lab::OptionalRef<Value> Find(const Key &key) {
    Node *current = root_;
    if (current == nullptr) {
      return lab::NullOpt;
    }

    while (true) {
      if (key > current->key) {
        // right subtree
        if (current->right == nullptr) {
          return lab::NullOpt;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          return lab::NullOpt;
        }
        current = current->left;
      } else /* if (key == current.key) */ {
        // current node
        return lab::MakeOptional(current->data);
      }
    }
  }

  const lab::OptionalRef<const Value> Find(const Key &key) const {
    Node *current = root_;
    if (current == nullptr) {
      return lab::NullOpt;
    }

    while (true) {
      if (key > current->key) {
        // right subtree
        if (current->right == nullptr) {
          return lab::NullOpt;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          return lab::NullOpt;
        }
        current = current->left;
      } else /* if (key == current.key) */ {
        // current node
        return lab::MakeOptional(current->data);
      }
    }
  }

 private:
  struct Node {
    Node(const Key &key, Value &&data)
        : key(key), data(data), left(nullptr), right(nullptr) {}
    template <typename... Args>
    Node(const Key &key, Args &&...args)
        : key(key),
          data(std::forward<Args>(args)...),
          left(nullptr),
          right(nullptr) {}
    Node(const Node &other)
        : key(other.key),
          data(other.data),
          left(other.left == nullptr ? nullptr : other.left->copy()),
          right(other.right == nullptr ? nullptr : other.right->copy()) {}
    ~Node() {
      delete left;
      delete right;
    }

    [[nodiscard]] Node *copy() const {
      auto *copy = new Node(*this);
      return copy;
    }

    Key key;
    Value data;

    Node *left;
    Node *right;
  };

  Node *root_;
  size_t count_{};

  template <typename F>
  auto CallOnNode(F &&func, Node *node) {
    if constexpr (std::is_invocable_v<F, Value &>) {
      return func(node->data);
    } else if constexpr (std::is_invocable_v<F, const Key &, Value &>) {
      return func(node->key, node->data);
    } else {
      static_assert(false,
                    "Function must be either auto(Value &) or auto(const Key "
                    "&, Value &).");
    }
  }
  template <typename F, typename T>
  T CallOnNode(F &&func, T previous, Node *node) {
    if constexpr (std::is_invocable_v<F, T, Value &>) {
      return func(previous, node->data);
    } else if constexpr (std::is_invocable_v<F, T, const Key &, Value &>) {
      return func(previous, node->key, node->data);
    } else {
      static_assert(false,
                    "Function must be either T(T, Value &) or T(T, const Key "
                    "&, Value &).");
    }
  }

  template <TreeTraversal Traversal, typename F>
  void TraverseInternal(F &&func, Node *node) {
    if (node == nullptr) {
      return;
    }

    if constexpr (Traversal == TreeTraversal::kPreOrder) {
      // NLR
      CallOnNode(std::forward<F>(func), node);
      TraverseInternal<Traversal>(std::forward<F>(func), node->left);
      TraverseInternal<Traversal>(std::forward<F>(func), node->right);
    } else if constexpr (Traversal == TreeTraversal::kPostOrder) {
      // LRN
      TraverseInternal<Traversal>(std::forward<F>(func), node->left);
      TraverseInternal<Traversal>(std::forward<F>(func), node->right);
      CallOnNode(std::forward<F>(func), node);
    } else if constexpr (Traversal == TreeTraversal::kInOrder) {
      // LNR
      TraverseInternal<Traversal>(std::forward<F>(func), node->left);
      CallOnNode(std::forward<F>(func), node);
      TraverseInternal<Traversal>(std::forward<F>(func), node->right);
    } else {
      static_assert(false, "Invalid traversal order.");
    }
  }

  template <TreeTraversal Traversal, typename T, typename F>
  T FoldInternal(F &&func, T seed, Node *node) {
    if (node == nullptr) {
      return seed;
    }

    if constexpr (Traversal == TreeTraversal::kPreOrder) {
      // NLR
      seed = CallOnNode(std::forward<F>(func), seed, node);
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->left);
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->right);
    } else if constexpr (Traversal == TreeTraversal::kPostOrder) {
      // LRN
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->left);
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->right);
      seed = CallOnNode(std::forward<F>(func), seed, node);
    } else if constexpr (Traversal == TreeTraversal::kInOrder) {
      // LNR
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->left);
      seed = CallOnNode(std::forward<F>(func), seed, node);
      seed = FoldInternal<Traversal>(std::forward<F>(func), seed, node->right);
    } else {
      static_assert(false, "Invalid traversal order.");
    }

    return seed;
  }

  void InsertInternal(const Key &key, Value &&value) {
    auto current = root_;
    while (true) {
      if (key > current->key) {
        // right subtree
        if (current->right == nullptr) {
          current->right = new Node(key, value);
          break;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          current->left = new Node(key, value);
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
          current->right = new Node(key, std::forward<Args>(args)...);
          break;
        }
        current = current->right;
      } else if (key < current->key) {
        // left subtree
        if (current->left == nullptr) {
          current->left = new Node(key, std::forward<Args>(args)...);
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
