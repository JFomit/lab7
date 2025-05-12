#ifndef LAB_HASHTABLE_H_
#define LAB_HASHTABLE_H_

#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "hasher.h"
#include "optional_ref.h"

namespace lab {
// A hashtable with open addressing and linear probing.
template <typename Key, typename Value, typename Hasher = Hasher<Key>,
          float LoadFactor = 0.8f>
class HashTable {
 protected:
  static constexpr float kLoadFactor = LoadFactor;
  // Contract: Only slots with `status == Status::kOccupied` store anything, all other are uninitialized.
  enum class Status : int8_t {
    kFree = 0,
    kOccupied = 1,
    kTombstone = 2,
  };
  struct Slot {
    Status status;
    alignas(alignof(Key)) std::byte key[sizeof(Key)];
    alignas(alignof(Value)) std::byte value[sizeof(Value)];

    Slot() : status(Status::kFree) {}
    explicit Slot(enum Status status) : status(status) {}
    Slot(Key &&key, Value &&value, enum Status status = Status::kOccupied)
        : status(status) {
      // TODO: support constexpr by switching to std::construct_at
      ::new ((void *)&this->key) Key(std::forward<Key>(key));
      ::new ((void *)&this->value) Value(std::forward<Value>(value));
    }
    Slot(const Key &&key, Value &&value, enum Status status = Status::kOccupied)
        : status(status) {
      ::new ((void *)&this->key) Key(std::forward<const Key>(key));
      ::new ((void *)&this->value) Value(std::forward<Value>(value));
    }

    const Key &GetKey() const {
      // Note: std::launder is needed after the change of object model in P0137R1
      return *std::launder(reinterpret_cast<const Key *>(&this->key));
    }
    Value &GetValue() {
      // Note: std::launder is needed after the change of object model in P0137R1
      return *std::launder(reinterpret_cast<Value *>(&this->value));
    }
    const Value &GetValue() const {
      // Note: std::launder is needed after the change of object model in P0137R1
      return *std::launder(reinterpret_cast<const Value *>(&this->value));
    }

    // Destroys inner values, i.e. key-value pair.
    void Destroy() {
      // Note: std::launder is needed after the change of object model in P0137R1
      std::destroy_at(std::launder(reinterpret_cast<Key *>(&this->key)));
      std::destroy_at(std::launder(reinterpret_cast<Value *>(&this->value)));
    }
  };
  static_assert(std::equality_comparable<Key>, "Key type must be comparable.");
  static_assert(std::is_copy_assignable_v<Key> &&
                    std::is_copy_constructible_v<Key>,
                "Key type must support copying.");
  static_assert(LoadFactor > 0 && LoadFactor < 1,
                "Load factor must be between 0 and 1 (exclusive).");

 public:
  HashTable() : buckets_(nullptr), capacity_(0), size_(0), hasher_({}) {}
  ~HashTable() {
    for (size_t i = 0; i < capacity_; ++i) {
      if (buckets_[i].status == Status::kOccupied) {
        buckets_[i].Destroy();
      }
    }
    size_ = 0;
    capacity_ = 0;
    delete[] buckets_;
  }

  [[nodiscard]] bool Empty() const { return size_ == 0; }

  void Insert(const Key &key, Value &&value) {
    EnsureCapacity(size_ + 1);

    InsertToBuckets(buckets_, capacity_, std::forward<const Key>(key),
                    std::forward<Value>(value));
    ++size_;
  }
  lab::OptionalRef<Value> Get(const Key &key) {
    // we remove const here as const version of Get actually returns a correct reference, which can be modified safely.
    return (lab::OptionalRef<Value>)std::as_const(*this)
        .HashTable<Key, Value, Hasher>::Get(std::forward<const Key>(key));
  }
  const lab::OptionalRef<Value> Get(const Key &key) const {
    size_t i = 0;
    auto default_hash = (size_t)hasher_(key);
    do {
      auto hash = (default_hash + i) % capacity_;
      switch (buckets_[hash].status) {
        case Status::kTombstone:
          ++i;
          continue;
        case Status::kFree:
          return lab::NullRef;
        case Status::kOccupied:
          if (buckets_[hash].GetKey() == key) {
            return lab::MakeOptional(buckets_[hash].GetValue());
          }
          ++i;
          continue;
      }
    } while (true);
  }
  bool Contains(const Key &key) const {
    return Get(std::forward<const Key>(key)).HasValue();
  }
  bool Remove(const Key &key) {
    size_t i = 0;
    auto default_hash = (size_t)hasher_(key);
    do {
      auto hash = (default_hash + i) % capacity_;
      switch (buckets_[hash].status) {
        case Status::kTombstone:
          ++i;
          continue;
        case Status::kFree:
          return false;
        case Status::kOccupied:
          if (buckets_[hash].GetKey() == key) {
            buckets_[hash].Destroy();
            buckets_[hash].status = Status::kTombstone;
            --size_;
            return true;
          }
          ++i;
          continue;
      }
    } while (true);
  }

  [[nodiscard]] constexpr float load_factor() const { return kLoadFactor; }
  [[nodiscard]] size_t size() const { return size_; }
  [[nodiscard]] size_t capacity() const { return capacity_; }
  [[nodiscard]] Hasher hasher() const { return hasher_; }

  struct Iter {
    Iter(Slot *buckets, size_t capacity)
        : buffer_(buckets), capacity_(capacity) {}

    const Value &Get() const {
      assert(buffer_[index_].status == Status::kOccupied);
      return buffer_[index_].GetValue();
    }
    bool Next() {
      if (buffer_[index_].status == Status::kOccupied) {
        return true;
      }
      while (true) {
        if (index_ >= capacity_) {
          return false;
        }
        ++index_;

        if (buffer_[index_].status == Status::kOccupied) {
          return true;
        }
      }
    }

   private:
    const Slot *buffer_;
    size_t capacity_;
    size_t index_{};
  };

  Iter GetIter() const { return Iter(buckets_, capacity_); }

 protected:
  [[nodiscard]] const Slot *storage() const { return buckets_; }

 private:
  inline static bool ShouldResize(size_t target, size_t actual) {
    return (float)target / (float)actual >= kLoadFactor;
  }

  void EnsureCapacity(size_t target) {
    if (buckets_ == nullptr) {
      assert(capacity_ == 0);
      assert(size_ == 0);

      capacity_ =
          std::bit_ceil((size_t)std::ceil(((float)target / kLoadFactor)));
      buckets_ = new Slot[capacity_];
      return;
    }

    if (!ShouldResize(target, capacity_)) {
      return;
    }

    auto next_capacity = capacity_;
    while (ShouldResize(target, next_capacity)) {
      next_capacity *= 2;
    }

    auto *next_buckets = new Slot[next_capacity];
    Slot *old_buckets = buckets_;
    for (size_t i = 0; i < capacity_; ++i) {
      if (old_buckets[i].status == Status::kOccupied) {
        InsertToBuckets(next_buckets, next_capacity, old_buckets[i].GetKey(),
                        std::move(old_buckets[i].GetValue()));
        // getting care of old data
        old_buckets[i].Destroy();
        old_buckets[i].status = Status::kTombstone;
      }
    }
    capacity_ = next_capacity;
    buckets_ = next_buckets;
    delete[] old_buckets;
  }

  // This method assumes that free place for a new entry exists and will indefinitely loop otherwise.
  void InsertToBuckets(Slot *buckets, size_t capacity, const Key &key,
                       Value &&value) {
    size_t i = 0;
    auto default_hash = (size_t)hasher_(key);
    do {
      auto hash = (default_hash + i) % capacity;
      if (buckets[hash].status == Status::kOccupied) {
        ++i;
        continue;
      }

      ::new ((void *)&buckets[hash])
          Slot(std::forward<const Key>(key), std::forward<Value>(value));

      break;
    } while (true);
  }

  Slot *buckets_;
  size_t capacity_;
  size_t size_;

  Hasher hasher_;
};
}  // namespace lab

#endif  // LAB_HASHTABLE_H_