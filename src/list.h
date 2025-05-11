#ifndef LAB_LIST_H_
#define LAB_LIST_H_

#include <cassert>
#include <compare>
#include <cstddef>
#include <type_traits>

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

   private:
    friend class List;

    ListNode *next_;
    ListNode *previous_;
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

  void push_back(const T &item) {
    auto *node = new ListNode(item);
    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous = tail_;
      tail_->next_ = node;
      tail_ = node;
    }
    ++count_;
  }
  void push_back(T &&item) {
    auto *node = new ListNode(item);
    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous = tail_;
      tail_->next_ = node;
      tail_ = node;
    }
    ++count_;
  }
  void push_front(const T &item) {
    auto *node = new ListNode(item);
    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next = head_;
      head_->previous_ = node;
      head_ = node;
    }
    ++count_;
  }
  void push_front(T &&item) {
    auto *node = new ListNode(item);
    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next = head_;
      head_->previous_ = node;
      head_ = node;
    }
    ++count_;
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    auto *node = new ListNode(args...);
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
      current = current->next;
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
      current = current->next;
    }
  }

 private:
  size_t count_;
  ListNode *head_;
  ListNode *tail_;
};
}  // namespace lab

#endif  // LAB_LIST_H_