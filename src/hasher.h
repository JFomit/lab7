#ifndef LAB_HASHER_H_
#define LAB_HASHER_H_

#include <cstdint>
#include <string>
namespace lab {
namespace detail {
// djb2 hash, see http://www.cse.yorku.ca/~oz/hash.html
constexpr uint64_t StringHash(const char *v) {
  const char *bytes = v;
  uint64_t hash = 5381;
  int c;

  while ((c = (unsigned char)*bytes++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}
}  // namespace detail

template <typename T>
struct Hasher {
  using Key = T;

  // Deafuls to FNV, see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
  constexpr uint64_t operator()(const Key &v) const {
    const char *bytes = reinterpret_cast<const char *>(&v);
    uint64_t hash = 0xCBF29CE484222325;
    const uint64_t kFnvPrime = 0x00000100000001B3;
    for (std::size_t i = 0; i < sizeof(v); ++i) {
      hash ^= bytes[i];
      hash *= kFnvPrime;
    }
    return hash;
  }
};

template <>
struct Hasher<std::string> {
  using Key = std::string;

  constexpr uint64_t operator()(const Key &v) const {
    return detail::StringHash(v.c_str());
  }
};
template <>
struct Hasher<char *> {
  using Key = char *;

  constexpr uint64_t operator()(const Key &v) const {
    return detail::StringHash(v);
  }
};
template <>
struct Hasher<const char *> {
  using Key = const char *;

  constexpr uint64_t operator()(const Key &v) const {
    return detail::StringHash(v);
  }
};
}  // namespace lab

#endif  // LAB_HASHER_H_