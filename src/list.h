#ifndef LAB_LIST_H_
#define LAB_LIST_H_

#include <cassert>
#include <type_traits>
namespace lab {
class List {
  using T = int;

 public:
  template <typename T>
  struct ListNode {
    T item;
    ListNode<T> *next;
    ListNode<T> *previous;

    ListNode(const T &v) : item(v), next(nullptr), previous(nullptr) {}
    ListNode(T &&v) : item(v), next(nullptr), previous(nullptr) {}
    template <typename... Args>
    ListNode(Args &&...args)
        : item(args...), next(nullptr), previous(nullptr) {}

    ~ListNode() = default;
    ListNode(const ListNode<T> &other) = delete;
    ListNode(ListNode<T> &&other) = delete;
  };

 public:
  List() : head_(nullptr), tail_(nullptr) {}
  ~List() {
    delete head_;
    delete tail_;
  }

  [[nodiscard]] ListNode<T> *head() const noexcept { return head_; }
  [[nodiscard]] ListNode<T> *tail() const noexcept { return tail_; }

  T &front() { return head_->item; }
  [[nodiscard]] const T &front() const { return head_->item; }
  T &back() { return tail_->item; }
  [[nodiscard]] const T &back() const { return tail_->item; }

  [[nodiscard]] bool empty() const noexcept { return head_ == tail_; }

  void push_back(const T &item) {
    auto *node = new ListNode<T>(item);
    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous = tail_;
      tail_->next = node;
      tail_ = node;
    }
  }
  void push_back(T &&item) {
    auto *node = new ListNode<T>(item);
    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous = tail_;
      tail_->next = node;
      tail_ = node;
    }
  }
  void push_front(const T &item) {
    auto *node = new ListNode<T>(item);
    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next = head_;
      head_->previous = node;
      head_ = node;
    }
  }
  void push_front(T &&item) {
    auto *node = new ListNode<T>(item);
    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next = head_;
      head_->previous = node;
      head_ = node;
    }
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    auto *node = new ListNode<T>(args...);
    if (tail_ == nullptr) {
      assert(head_ == nullptr);
      tail_ = node;
      head_ = node;
    } else {
      node->previous = tail_;
      tail_->next = node;
      tail_ = node;
    }
  }
  template <typename... Args>
  void emplace_front(Args &&...args) {
    auto *node = new ListNode<T>(args...);
    if (head_ == nullptr) {
      assert(tail_ == nullptr);
      head_ = node;
      tail_ = node;
    } else {
      node->next = head_;
      tail_->next = node;
      head_ = node;
    }
  }

  template <typename F>
  ListNode<T> *find(F &&predicate) const
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
  ListNode<T> *find_last(F &&predicate) const
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
  ListNode<T> *head_;
  ListNode<T> *tail_;
};
}  // namespace lab

#endif  // LAB_LIST_H_