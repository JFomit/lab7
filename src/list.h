#ifndef LAB_LIST_H_
#define LAB_LIST_H_

#include <cassert>
#include <compare>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace lab {
template <typename T>
class List {
 public:
  struct ListNode {
    T item;

    ListNode(const T &v) : item(v), next_(nullptr), previous_(nullptr) {}
    ListNode(T &&v) : item(v), next_(nullptr), previous_(nullptr) {}
    template <typename... Args>
    ListNode(Args &&...args)
        : item(args...), next_(nullptr), previous_(nullptr) {}

    ~ListNode() = default;
    ListNode(const ListNode &other) = delete;
    ListNode(ListNode &&other) = delete;

    ListNode *next() const { return next_; }
    ListNode *previous() const { return previous_; }

   private:
    friend class List;

    ListNode *next_;
    ListNode *previous_;

#if DEBUG
    List *owner_ = nullptr;
#endif
  };

 public:
  List() : count_(0), head_(nullptr), tail_(nullptr) {}
  ~List() {
    while (!empty()) {
      pop_front();
    }
    delete head_;
    delete tail_;
    head_ = nullptr;
    tail_ = nullptr;
    count_ = 0;
  }

  [[nodiscard]] ListNode *head() const noexcept { return head_; }
  [[nodiscard]] ListNode *tail() const noexcept { return tail_; }

  T &front() { return head_->item; }
  [[nodiscard]] const T &front() const { return head_->item; }
  T &back() { return tail_->item; }
  [[nodiscard]] const T &back() const { return tail_->item; }

  [[nodiscard]] size_t count() const noexcept { return count_; }
  [[nodiscard]] bool empty() const noexcept { return count_ == 0; }

  void push_back(T &&item) {
    auto *node = new ListNode(item);
#if DEBUG
    node->owner_ = this;
#endif

    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous_ = tail_;
      tail_->next_ = node;
      tail_ = node;
    }
    ++count_;
  }
  void push_front(T &&item) {
    auto *node = new ListNode(item);
#if DEBUG
    node->owner_ = this;
#endif

    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next_ = head_;
      head_->previous_ = node;
      head_ = node;
    }
    ++count_;
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    auto *node = new ListNode(args...);
#if DEBUG
    node->owner_ = this;
#endif

    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous_ = tail_;
      tail_->next_ = node;
      tail_ = node;
    }
    ++count_;
  }
  template <typename... Args>
  void emplace_front(Args &&...args) {
    auto *node = new ListNode(args...);
#if DEBUG
    node->owner_ = this;
#endif

    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next_ = head_;
      tail_->next_ = node;
      head_ = node;
    }
    ++count_;
  }

  void pop_front() {
    assert(count_ > 0 && "Tried to call pop_front on empty list.");
    assert(head_ != nullptr);

    if (head_ == tail_) {
      assert(count_ == 1);
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      --count_;
      return;
    }

    auto *old = head_;
    head_ = head_->next_;
    delete old;
    head_->previous_ = nullptr;
    --count_;
  }
  void pop_back() {
    assert(count_ > 0 && "Tried to call pop_back on empty list.");
    assert(tail_ != nullptr);

    if (head_ == tail_) {
      assert(count_ == 1);
      delete tail_;
      head_ = nullptr;
      tail_ = nullptr;
      --count_;
      return;
    }

    auto *old = tail_;
    tail_ = tail_->previous_;
    delete old;
    tail_->next_ = nullptr;
    --count_;
  }

  // Returns: the new node.
  ListNode *insert_before(ListNode *marker, T &&value) {
#if DEBUG
    assert(marker->owner_ == this &&
           "Tried to modify a node of other List<T>.");
#endif
    assert(!empty() && "Somehow got a pointer to a node of an empty list.");

    if (marker == head_) {
      push_front(std::forward<T>(value));
      return head();
    }

    auto before = marker->previous_;
    auto *node = new ListNode(std::forward<T>(value));
#if DEBUG
    node->owner_ = this;
#endif
    node->next_ = marker;
    node->previous_ = before;
    marker->previous_ = node;
    before->next_ = node;
    ++count_;

    return node;
  }
  // Returns: the new node.
  ListNode *insert_after(ListNode *marker, T &&value) {
#if DEBUG
    assert(marker->owner_ == this &&
           "Tried to modify a node of other List<T>.");
#endif
    assert(!empty() && "Somehow got a pointer to a node of an empty list.");

    if (marker == tail_) {
      push_back(std::forward<T>(value));
      return tail();
    }

    auto after = marker->next_;
    auto *node = new ListNode(std::forward<T>(value));
#if DEBUG
    node->owner_ = this;
#endif
    node->previous_ = marker;
    node->next_ = after;
    after->previous_ = node;
    marker->next_ = node;
    ++count_;

    return node;
  }
  // Returns: true, is `first` points to an element, preceding element, pointed to by `second`; false, otherwise.
  // This method also returns false if either of the arguments or both are `nullptr`.
  bool is_forward_reachable(ListNode *first, ListNode *second) const {
    assert(first->owner_ == this && "Tried to use node from other List<T>.");
    assert(second->owner_ == this && "Tried to use node from other List<T>.");

    if (first == nullptr || second == nullptr) {
      return false;
    }
    if (first == second) {
      return true;
    }

    auto *start = first;
    while (start != second) {
      if (start == nullptr) {
        return false;
      }
      start = start->next_;
    }

    return true;
  }

  // Node: if begin == end, inserts after it.
  void replace_range(ListNode *begin, ListNode *end, const List<T> &values) {
#if DEBUG
    assert(begin->owner_ == this && "Tried to use node from other List<T>.");
    assert(end->owner_ == this && "Tried to use node from other List<T>.");
    assert(&values != this && "Can't insert to itself.");

    // reachability test
    {
      auto *start = begin;
      while (start != end) {
        assert(start != nullptr && "end before begin.");
        start = start->next_;
      }
    }
#endif

    remove_range(begin, end);
    auto *current = begin;
    values.for_each([this, &current](auto &item) {
      current = insert_after(current, std::forward<T>(item));
    });
  }

  void clear() {
    while (!empty()) {
      pop_back();
    }
    assert(count_ == 0);
  }

  void remove_range(ListNode *begin, ListNode *end) {
#if DEBUG
    assert(begin->owner_ == this && "Tried to use node from other List<T>.");
    assert(end->owner_ == this && "Tried to use node from other List<T>.");

    // reachability test
    {
      auto *start = begin;
      while (start != end) {
        assert(start != nullptr && "end before begin.");
        start = start->next_;
      }
    }
#endif
    if (begin == end) {
      return;
    }
    if (begin == end->previous_) {
      assert(begin->next_ == end);
      return;
    }

    size_t count = 0;
    auto before = begin;
    auto after = end;

    auto iterator = begin->next_;
    while (true) {
      if (iterator == end->previous_) {
        ++count;
        delete iterator;
        break;
      }

      auto next = iterator->next_;
      delete iterator;
      ++count;
      iterator = next;
    }

    if (before != nullptr) {
      before->next_ = after;
    }
    if (after != nullptr) {
      after->previous_ = before;
    }

    assert(count <= count_ && "Deleted too much.");
    count_ -= count;
  }

  ListNode *min() const {
    static_assert(std::three_way_comparable<T>,
                  "For min()/max() to work T must be comparable.");
    if (empty()) {
      return nullptr;
    }

    auto best = head_->item;
    auto best_node = head_;

    auto *current = head_;
    while (current != nullptr) {
      if (current->item < best) {
        best_node = current;
        best = current->item;
      }
      current = current->next_;
    }

    return best_node;
  }
  ListNode *max() const {
    static_assert(std::three_way_comparable<T>,
                  "For min()/max() to work T must be comparable.");
    if (empty()) {
      return nullptr;
    }

    auto best = head_->item;
    auto best_node = head_;

    auto *current = head_;
    while (current != nullptr) {
      if (current->item > best) {
        best_node = current;
        best = current->item;
      }
      current = current->next_;
    }

    return best_node;
  }

  template <typename F>
  ListNode *find(F &&predicate) const
      noexcept(std::is_nothrow_invocable_v<F, T &&>) {
    auto *current = head_;
    while (current != nullptr) {
      if (predicate(current->item)) {
        return current;
      }

      current = current->next;
    }

    return nullptr;
  }
  template <typename F>
  ListNode *find_last(F &&predicate) const
      noexcept(std::is_nothrow_invocable_v<F, T &&>) {
    auto *current = tail_;
    while (current != nullptr) {
      if (predicate(current->item)) {
        return current;
      }

      current = current->previous;
    }

    return nullptr;
  }

  template <typename F>
  void for_each(F &&selector) const
      noexcept(std::is_nothrow_invocable_v<F, T &&>) {
    auto *current = head_;
    while (current != nullptr) {
      selector(current->item);
      current = current->next_;
    }
  }

 private:
  size_t count_;
  ListNode *head_;
  ListNode *tail_;
};
}  // namespace lab

#endif  // LAB_LIST_H_