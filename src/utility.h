#ifndef LAB_UTILITY_H_
#define LAB_UTILITY_H_

namespace lab {
template <typename First, typename Second>
struct Pair {
  First first;
  Second second;

  Pair() = default;
  ~Pair() = default;
  Pair(First &&first, Second &&second) : first(first), second(second) {}
};

template <typename First, typename Second>
Pair<First, Second> MakePair(First &&first, Second &&second) {
  return {first, second};
}
}  // namespace lab

#endif  // LAB_UTILITY_H_