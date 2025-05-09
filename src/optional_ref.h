#ifndef LAB_OPTIONAL_REF_H_
#define LAB_OPTIONAL_REF_H_

#include <optional>
namespace lab {

namespace detail {
struct NullRef {};
}  // namespace detail

template <typename T>
struct OptionalRef {
 public:
  constexpr OptionalRef(T *const ptr) : kRef(ptr) {}
  constexpr OptionalRef(detail::NullRef) : kRef(nullptr) {}

  [[nodiscard]] constexpr bool HasValue() const { return kRef != nullptr; }
  constexpr const T &Get() const {
    if (HasValue()) {
      return *kRef;
    }

    throw std::bad_optional_access();
  }
  constexpr T &Get() {
    if (HasValue()) {
      return *kRef;
    }

    throw std::bad_optional_access();
  }

 private:
  T *const kRef;
};

detail::NullRef NullOpt;

template <typename T>
OptionalRef<T> MakeOptional(T &ref) {
  return OptionalRef<T>(&ref);
}
template <typename T>
OptionalRef<const T> MakeOptional(const T &ref) {
  return OptionalRef<const T>(&ref);
}
}  // namespace lab

#endif  // LAB_OPTIONAL_REF_H_