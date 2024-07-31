#include <iostream>
#include <vector>

template<typename T, typename Compare = std::less<T>>
requires std::default_initializable<T> && std::swappable<T>
class BinHeap {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const pointer;

  using size_type = std::vector<T>::size_type;
  using difference_type = std::ptrdiff_t;

  BinHeap() = default;

  template<class U>
  void Insert(U&& x) {
    storage_.emplace_back(std::forward<U>(x));
    SiftUp(Size());
  }

  size_type Size() const { return storage_.size() - 1; }

  reference Top() { return storage_[1]; }
  void ExtractMin();

  ~BinHeap() = default;

 private:
  [[no_unique_address]] Compare comparator_;

  std::vector<T> storage_{0};

  void SiftUp(size_t v);
  void SiftDown(size_t v);
  void Exchange(size_t u, size_t v);
};

template<typename T, typename Compare>
requires std::default_initializable<T> && std::swappable<T>
void BinHeap<T, Compare>::ExtractMin() {
  Exchange(1, Size());
  SiftDown(1);
  storage_.pop_back();
}

template<typename T, typename Compare>
requires std::default_initializable<T> && std::swappable<T>
void BinHeap<T, Compare>::Exchange(size_t u, size_t v) {
  std::swap(storage_[u], storage_[v]);
}

template<typename T, typename Compare>
requires std::default_initializable<T> && std::swappable<T>
void BinHeap<T, Compare>::SiftDown(size_t v) {
  while (v * 2 <= Size()) {
    size_t u = v * 2;
    if (v * 2 + 1 <= Size() && comparator_(storage_[v * 2 + 1], storage_[u])) {
      u = v * 2 + 1;
    }
    if (!comparator_(storage_[u], storage_[v])) {
      break;
    }
    Exchange(u, v);
    v = u;
  }
}

template<typename T, typename Compare>
requires std::default_initializable<T> && std::swappable<T>
void BinHeap<T, Compare>::SiftUp(size_t v) {
  while (v != 1 && comparator_(storage_[v], storage_[v / 2])) {
    Exchange(v, v / 2);
    v /= 2;
  }
}