#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
/**
 * @file deque.hpp
 * @author of code: Hermes
 * @date of submit 19.03.2023
 */

template <typename Type, typename Allocator = std::allocator<Type>>
class Deque {
 public:
  using allocator_type = Allocator;

  Deque();
  Deque(size_t count, const Type& value, const Allocator& alloc = Allocator());
  Deque(std::initializer_list<Type> init, const Allocator& alloc = Allocator());
  Deque(const Allocator& alloc);
  Deque(Deque&& other) noexcept;
  Deque(size_t count, const Allocator& alloc = Allocator());
  Deque(const Deque& other_deque);

  Deque& operator=(const Deque& other_deque);
  Deque& operator=(Deque&& other) noexcept;

  ~Deque() { clear_ptr_arr(); }

  [[nodiscard]] size_t size() const;

  Type& operator[](size_t ind);

  const Type& operator[](size_t ind) const;

  bool empty();

  Type& at(size_t ind);

  const Type& at(size_t ind) const;
  template <typename... Args>
  void emplace_back(Args&&... args);
  template <typename... Args>
  void emplace_front(Args&&... args);
  void push_back(const Type& value);
  void push_back(Type&& value);
  void push_front(Type&& value);
  void pop_back();

  void push_front(const Type& value);

  void pop_front();

  template <bool IsConst>
  class Iterator;

  template <bool IsConst>
  using based_reverse = std::reverse_iterator<Iterator<IsConst>>;

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = based_reverse<false>;
  using const_reverse_iterator = based_reverse<true>;

  iterator begin() { return iterator(ptr_arr_ + front_.ptr, front_.ind); }

  const_iterator begin() const {
    return const_iterator(ptr_arr_ + front_.ptr, front_.ind);
  }

  const_iterator cbegin() const {
    return const_iterator(ptr_arr_ + front_.ptr, front_.ind);
  }

  iterator end() {
    Index index = get_index(size_);
    return iterator(ptr_arr_ + index.ptr, index.ind);
  }

  const_iterator end() const {
    Index index = get_index(size_);
    return const_iterator(ptr_arr_ + index.ptr, index.ind);
  }

  const_iterator cend() const {
    Index index = get_index(size_);
    return const_iterator(ptr_arr_ + index.ptr, index.ind);
  }

  reverse_iterator rbegin() {
    iterator iter = end();
    return reverse_iterator(iter);
  }

  const_reverse_iterator rbegin() const {
    iterator iter = end();
    const_iterator iter_const = const_iterator(iter);
    return const_reverse_iterator(iter_const);
  }

  const_reverse_iterator crbegin() const {
    iterator iter = end();
    const_iterator iter_const = const_iterator(iter);
    return const_reverse_iterator(iter_const);
  }

  reverse_iterator rend() {
    iterator iter = begin();
    return reverse_iterator(iter);
  }

  const_reverse_iterator rend() const {
    const_iterator iter = cbegin();
    return const_reverse_iterator(iter);
  }

  const_reverse_iterator crend() const {
    const_iterator iter = cbegin();
    return const_reverse_iterator(iter);
  }

  template <bool IsConst>
  void insert(Iterator<IsConst> iter, const Type& value);

  template <bool IsConst>
  void erase(Iterator<IsConst> iter);
  template <bool IsConst, typename... Args>
  void emplace(const Iterator<IsConst>& pos, Args&&... args);
  const allocator_type& get_allocator();

 private:
  static const size_t kMaxSize = 32;
  struct Index {
    size_t ptr;
    size_t ind;
  };
  using allocator_traits = std::allocator_traits<allocator_type>;
  using rebind_allocator =
      typename allocator_traits::template rebind_alloc<Type*>;
  using rebind_allocator_traits = std::allocator_traits<rebind_allocator>;
  allocator_type true_alloc_;
  rebind_allocator alloc_;
  Type** ptr_arr_;
  size_t size_;
  size_t ptr_capacity_;
  size_t first_added_;
  size_t last_added_;
  Index front_;

  void reserve_back();

  void reserve_front();

  void clear_ptr_arr();

  void swap(Deque<Type>& other_deque);

  Index get_index(size_t index) const;
};
template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque()
    : ptr_arr_(rebind_allocator_traits::allocate(alloc_, 1)),
      size_(0),
      ptr_capacity_(1),
      first_added_(0),
      last_added_(0),
      front_({0, 0}) {
  ptr_arr_[0] = allocator_traits::allocate(true_alloc_, kMaxSize);
}
template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(std::initializer_list<Type> init,
                              const Allocator& alloc)
    : true_alloc_(alloc),
      alloc_(rebind_allocator(alloc)),
      size_(init.size()),
      ptr_capacity_((size_ + kMaxSize - 1) / kMaxSize) {
  ptr_arr_ = rebind_allocator_traits::allocate(alloc_, ptr_capacity_);
  first_added_ = 0;
  front_ = {0, 0};
  last_added_ = (ptr_capacity_ > 0) ? ptr_capacity_ - 1 : first_added_;
  auto iter = init.begin();
  for (size_t row = 0; row < ptr_capacity_; ++row) {
    // В конструкторе при исключении, он полностью должен удалиться, поскольку
    // он не полностью создается.
    try {
      ptr_arr_[row] = allocator_traits::allocate(true_alloc_, kMaxSize);
    } catch (...) {
      while (row > 0) {
        allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
        --row;
      }
    }
  }
  for (size_t i = 0; i < size_; ++i, ++iter) {
    Index index = get_index(i);
    try {
      allocator_traits ::construct(true_alloc_, ptr_arr_[index.ptr] + index.ind,
                                   *(iter));
    } catch (...) {
      while (i > 0) {
        allocator_traits::destroy(true_alloc_, ptr_arr_[index.ptr] + index.ind);
        --i;
      }
      for (size_t row = first_added_; row <= last_added_; ++row) {
        allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
      }
      rebind_allocator_traits ::deallocate(alloc_, ptr_arr_, ptr_capacity_);
    }
  }
}
template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(Deque&& other) noexcept
    : true_alloc_(std::move(other.true_alloc_)),
      alloc_(std::move(other.alloc_)),
      ptr_arr_(other.ptr_arr_),
      size_(other.size_),
      ptr_capacity_(other.ptr_capacity_),
      first_added_(other.first_added_),
      last_added_(other.last_added_),
      front_(other.front_) {
  other.ptr_arr_ = nullptr;
  other.size_ = 0;
  other.ptr_capacity_ = 0;
  other.front_.ptr = 0;
  other.front_.ind = 0;
  other.first_added_ = 0;
  other.last_added_ = 0;
}
template <typename Type, typename Allocator>
const Allocator& Deque<Type, Allocator>::get_allocator() {
  return true_alloc_;
}
template <typename Type, typename Allocator>
Deque<Type, Allocator>& Deque<Type, Allocator>::operator=(
    Deque&& other) noexcept {
  size_ = std::move_if_noexcept(other.size_);
  ptr_arr_ = std::move_if_noexcept(other.ptr_arr_);
  front_ = std::move_if_noexcept(other.front_);
  ptr_capacity_ = std::move_if_noexcept(other.ptr_capacity_);
  first_added_ = std::move_if_noexcept(other.first_added_);
  last_added_ = std::move_if_noexcept(other.last_added_);
  return *this;
}
template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(size_t count, const Allocator& alloc)
    : true_alloc_(alloc),
      alloc_(rebind_allocator(alloc)),
      size_(count),
      ptr_capacity_((count + kMaxSize - 1) / kMaxSize),
      first_added_(0),
      last_added_((ptr_capacity_ > 0) ? ptr_capacity_ - 1 : first_added_),
      front_({0, 0}) {
  // В конструкторе при исключении, он полностью должен удалиться, поскольку он
  // не полностью создается.
  try {
    ptr_arr_ = rebind_allocator_traits ::allocate(alloc_, ptr_capacity_);
  } catch (...) {
    throw;
  }
  for (size_t row = 0; row < ptr_capacity_; ++row) {
    try {
      ptr_arr_[row] = allocator_traits::allocate(true_alloc_, kMaxSize);
    } catch (...) {
      while (row > 0) {
        allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
        --row;
      }
      rebind_allocator_traits ::deallocate(alloc_, ptr_arr_, ptr_capacity_);
      throw;
    }
  }
  for (size_t ind = 0; ind < size_; ++ind) {
    Index index = get_index(ind);
    try {
      allocator_traits::construct(true_alloc_, ptr_arr_[index.ptr] + index.ind);
    } catch (...) {
      while (ind > 0) {
        index = get_index(ind);
        allocator_traits::destroy(true_alloc_, ptr_arr_[index.ptr] + index.ind);
        --ind;
      }
      for (size_t row = first_added_; row <= last_added_; ++row) {
        allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
      }
      rebind_allocator_traits ::deallocate(alloc_, ptr_arr_, ptr_capacity_);
      throw;
    }
  }
}
template <typename Type, typename Allocator>
void Deque<Type, Allocator>::push_front(Type&& value) {
  try {
    emplace_front(std::move(value));
  } catch (...) {
    throw;
  }
}
template <typename Type, typename Allocator>
void Deque<Type, Allocator>::push_back(Type&& value) {
  try {
    emplace_back(std::move(value));
  } catch (...) {
    throw;
  }
}
template <typename Type, typename Allocator>
template <typename... Args>
void Deque<Type, Allocator>::emplace_front(Args&&... args) {
  try {
    reserve_front();
    if (front_.ind == 0) {
      if (front_.ptr - 1 < first_added_) {
        try {
          ptr_arr_[front_.ptr - 1] =
              allocator_traits::allocate(true_alloc_, kMaxSize);
        } catch (...) {
          throw;
        }
        --first_added_;
      }
      try {
        allocator_traits::construct(true_alloc_,
                                    ptr_arr_[front_.ptr - 1] + kMaxSize - 1,
                                    std::forward<Args>(args)...);
      } catch (...) {
        allocator_traits::deallocate(
            true_alloc_, ptr_arr_[front_.ptr - 1] + kMaxSize - 1, kMaxSize);
        throw;
      }
      --front_.ptr;
      front_.ind = kMaxSize - 1;
    } else {
      try {
        allocator_traits::construct(true_alloc_,
                                    ptr_arr_[front_.ptr] + front_.ind - 1,
                                    std::forward<Args>(args)...);
      } catch (...) {
        allocator_traits::deallocate(
            true_alloc_, ptr_arr_[front_.ptr] + front_.ind - 1, kMaxSize);
        throw;
      }
      --front_.ind;
    }
    ++size_;
  } catch (...) {
    throw;
  }
}
template <typename Type, typename Allocator>
template <typename... Args>
void Deque<Type, Allocator>::emplace_back(Args&&... args) {
  try {
    emplace(end(), std::forward<Args>(args)...);
  } catch (...) {
    throw;
  }
}

template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(const Allocator& alloc)
    : true_alloc_(alloc),
      alloc_(rebind_allocator(alloc)),
      ptr_arr_(rebind_allocator_traits::allocate(alloc_, 1)),
      size_(0),
      ptr_capacity_(1),
      first_added_(0),
      last_added_(0),
      front_({0, 0}) {
  allocator_traits::construct(alloc_, ptr_arr_[0], 0);
}

template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(size_t count, const Type& value,
                              const Allocator& alloc)
    : true_alloc_(alloc),
      alloc_(rebind_allocator(alloc)),
      size_(count),
      ptr_capacity_((count + kMaxSize - 1) / kMaxSize),
      first_added_(0),
      last_added_((ptr_capacity_ > 0) ? ptr_capacity_ - 1 : first_added_),
      front_({0, 0}) {
  try {
    ptr_arr_ = rebind_allocator_traits ::allocate(alloc_, ptr_capacity_);
    for (size_t row = 0; row < ptr_capacity_; ++row) {
      ptr_arr_[row] = allocator_traits::allocate(true_alloc_, kMaxSize);
    }
    for (size_t ind = 0; ind < size_; ++ind) {
      Index index = get_index(ind);
      allocator_traits ::construct(true_alloc_, ptr_arr_[index.ptr] + index.ind,
                                   value);
    }
  } catch (...) {
    clear_ptr_arr();
    throw;
  }
}
template <typename Type, typename Allocator>
Deque<Type, Allocator>::Deque(const Deque& other_deque)
    : true_alloc_(
          rebind_allocator_traits::select_on_container_copy_construction(
              other_deque.true_alloc_)),
      alloc_(rebind_allocator_traits::select_on_container_copy_construction(
          other_deque.alloc_)),
      ptr_arr_(
          rebind_allocator_traits::allocate(alloc_, other_deque.ptr_capacity_)),
      size_(other_deque.size_),
      ptr_capacity_(other_deque.ptr_capacity_),
      first_added_(other_deque.first_added_),
      last_added_(other_deque.last_added_),
      front_(other_deque.front_) {
  size_t ptr_last =
      other_deque.get_index((other_deque.size_ > 1) ? other_deque.size_ - 1 : 0)
          .ptr;
  for (size_t row = other_deque.front_.ptr; row <= ptr_last; ++row) {
    try {
      ptr_arr_[row] = allocator_traits::allocate(true_alloc_, kMaxSize);
    } catch (...) {
      while (row > 0) {
        allocator_traits ::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
        --row;
      }

      throw;
    }
  }
  for (size_t ind = 0; ind < other_deque.size_; ++ind) {
    Index index = get_index(ind);
    try {
      allocator_traits::construct(true_alloc_, ptr_arr_[index.ptr] + index.ind,
                                  other_deque.ptr_arr_[index.ptr][index.ind]);
    } catch (...) {
      while (ind > 0) {
        allocator_traits::destroy(true_alloc_, ptr_arr_[index.ptr] + index.ind);
        --ind;
      }
      for (size_t row = first_added_; row <= last_added_; ++row) {
        allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
      }
      rebind_allocator_traits ::deallocate(alloc_, ptr_arr_, ptr_capacity_);
      throw;
    }
  }
}

template <typename Type, typename Allocator>
Deque<Type, Allocator>& Deque<Type, Allocator>::operator=(
    const Deque& other_deque) {
  if (rebind_allocator_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = other_deque.alloc_;
    true_alloc_ = other_deque.true_alloc_;
  }
  size_t count = 0;
  auto iter = begin();
  for (auto& other_it : other_deque) {
    if (count < size_) {
      *iter = other_it;
      ++iter;
      ++count;
      continue;
    }
    try {
      push_back(other_it);
      ++count;
    } catch (...) {
      clear_ptr_arr();
    }
  }
  while (size_ > other_deque.size_) {
    pop_back();
  }
  size_ = other_deque.size_;
  return *this;
}

template <typename Type, typename Allocator>
size_t Deque<Type, Allocator>::size() const {
  return size_;
}

template <typename Type, typename Allocator>
Type& Deque<Type, Allocator>::operator[](size_t ind) {
  Index index = get_index(ind);
  return ptr_arr_[index.ptr][index.ind];
}

template <typename Type, typename Allocator>
const Type& Deque<Type, Allocator>::operator[](size_t ind) const {
  Index index = get_index(ind);
  return ptr_arr_[index.ptr][index.ind];
}

template <typename Type, typename Allocator>
Type& Deque<Type, Allocator>::at(size_t ind) {
  if (ind >= size_) {
    throw std::out_of_range("Deque overflow");
  }
  Index index = get_index(ind);
  return ptr_arr_[index.ptr][index.ind];
}

template <typename Type, typename Allocator>
const Type& Deque<Type, Allocator>::at(size_t ind) const {
  if (ind >= size_) {
    throw std::out_of_range("Deque overflow");
  }
  Index index = get_index(ind);
  return ptr_arr_[index.ptr][index.ind];
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::push_back(const Type& value) {
  try {
    emplace_back(value);
  } catch (...) {
    throw;
  }
}
template <typename Type, typename Allocator>
template <bool IsConst, typename... Args>
void Deque<Type, Allocator>::emplace(const Deque::Iterator<IsConst>& pos,
                                     Args&&... args) {
  size_t ind = const_cast<Deque::Iterator<IsConst>&>(pos) - begin();
  reserve_back();
  Index index = get_index(size_);
  if (index.ind == 0) {
    if (index.ptr > last_added_) {
      try {
        ptr_arr_[index.ptr] = allocator_traits::allocate(true_alloc_, kMaxSize);
      } catch (...) {
        throw;
      }
      ++last_added_;
    }
  }
  try {
    allocator_traits ::construct(true_alloc_, ptr_arr_[index.ptr] + index.ind,
                                 std::forward<Args>(args)...);
  } catch (...) {
    if (index.ind == 0) {
      allocator_traits::deallocate(true_alloc_, ptr_arr_[index.ptr], kMaxSize);
    }
    throw;
  }
  ++size_;
  auto end_iter = end();
  --end_iter;
  index = get_index(ind);
  Iterator<IsConst> iter_elem(ptr_arr_ + index.ptr, index.ind);
  while (end_iter != iter_elem) {
    auto tmp = std::move(iter_elem);
    iter_elem = std::move((end_iter - 1));
    (end_iter - 1) = std::move(tmp);
    --end_iter;
  }
}
template <typename Type, typename Allocator>
template <bool IsConst>
void Deque<Type, Allocator>::insert(Deque::Iterator<IsConst> iter,
                                    const Type& value) {
  try {
    emplace(iter, value);
  } catch (...) {
    throw;
  }
}
template <typename Type, typename Allocator>
void Deque<Type, Allocator>::pop_back() {
  if (empty()) {
    throw std::out_of_range("deque is empty");
  }
  --size_;
  Index index = get_index(size_);
  allocator_traits::destroy(true_alloc_, ptr_arr_[index.ptr] + index.ind);
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::push_front(const Type& value) {
  reserve_front();
  if (front_.ind == 0) {
    if (front_.ptr - 1 < first_added_) {
      try {
        ptr_arr_[front_.ptr - 1] =
            allocator_traits::allocate(true_alloc_, kMaxSize);
      } catch (...) {
        throw;
      }
      --first_added_;
    }
    try {
      allocator_traits::construct(
          true_alloc_, ptr_arr_[front_.ptr - 1] + kMaxSize - 1, value);
    } catch (...) {
      allocator_traits::deallocate(true_alloc_,
                                   ptr_arr_[front_.ptr - 1] + kMaxSize - 1, 1);
      throw;
    }
    --front_.ptr;
    front_.ind = kMaxSize - 1;
  } else {
    try {
      allocator_traits::construct(true_alloc_,
                                  ptr_arr_[front_.ptr] + front_.ind - 1, value);
    } catch (...) {
      allocator_traits::deallocate(true_alloc_,
                                   ptr_arr_[front_.ptr - 1] + kMaxSize - 1, 1);
      throw;
    }
    --front_.ind;
  }
  ++size_;
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::pop_front() {
  if (empty()) {
    throw std::out_of_range("deque is empty");
  }
  allocator_traits::destroy(true_alloc_, ptr_arr_[front_.ptr] + front_.ind);
  Index index = get_index(1);
  front_.ptr = index.ptr;
  front_.ind = index.ind;
  --size_;
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::reserve_back() {
  Index end_index = {front_.ptr, front_.ind};
  if (size_ != 0) {
    end_index = get_index(size_ - 1);
  }
  if (size_ == 0 || end_index.ptr != ptr_capacity_ - 1 ||
      end_index.ind != kMaxSize - 1) {
    return;
  }
  try {
    Type** new_arr_ptr =
        rebind_allocator_traits::allocate(alloc_, 2 * ptr_capacity_);
    size_t amount_of_rows = last_added_ - first_added_ + 1;
    size_t begin = (2 * ptr_capacity_ - amount_of_rows) / 2;
    for (size_t row = begin; row < amount_of_rows + begin; ++row) {
      new_arr_ptr[row] = ptr_arr_[first_added_ + row - begin];
    }
    rebind_allocator_traits::deallocate(alloc_, ptr_arr_, ptr_capacity_);
    ptr_arr_ = new_arr_ptr;
    ptr_capacity_ *= 2;
    front_.ptr = begin + (front_.ptr - first_added_);
    first_added_ = begin;
    last_added_ = begin + amount_of_rows - 1;
  } catch (...) {
    clear_ptr_arr();
  }
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::reserve_front() {
  if (front_.ptr != 0 || front_.ind != 0) {
    return;
  }
  try {
    Type** new_arr_ptr =
        rebind_allocator_traits::allocate(alloc_, 2 * ptr_capacity_);
    size_t amount_of_rows = last_added_ - first_added_ + 1;
    size_t begin = std::max(static_cast<size_t>(1UL),
                            (2 * ptr_capacity_ - amount_of_rows) / 2);
    for (size_t row = begin; row < amount_of_rows + begin; ++row) {
      new_arr_ptr[row] = ptr_arr_[first_added_ + row - begin];
    }
    rebind_allocator_traits::deallocate(alloc_, ptr_arr_, ptr_capacity_);
    ptr_arr_ = new_arr_ptr;
    ptr_capacity_ *= 2;
    front_.ptr = begin + (front_.ptr - first_added_);
    first_added_ = begin;
    last_added_ = begin + amount_of_rows - 1;
  } catch (...) {
    clear_ptr_arr();
    throw;
  }
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::clear_ptr_arr() {
  if (ptr_arr_ != nullptr) {
    for (size_t ind = 0; ind < size_; ++ind) {
      Index index = get_index(ind);
      allocator_traits ::destroy(true_alloc_, ptr_arr_[index.ptr] + index.ind);
    }
    for (size_t row = first_added_; row <= last_added_; ++row) {
      allocator_traits::deallocate(true_alloc_, ptr_arr_[row], kMaxSize);
    }
    rebind_allocator_traits ::deallocate(alloc_, ptr_arr_, ptr_capacity_);
  }
}

template <typename Type, typename Allocator>
void Deque<Type, Allocator>::swap(Deque<Type>& other_deque) {
  if (rebind_allocator_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = other_deque.alloc_;
    true_alloc_ = other_deque.true_alloc_;
  }
  auto tmp = std::move(ptr_arr_);
  ptr_arr_ = std::move(other_deque.ptr_arr_);
  other_deque.ptr_arr_ = std::move(tmp);
  std::swap(size_, other_deque.size_);
  std::swap(front_, other_deque.front_);
  std::swap(ptr_capacity_, other_deque.ptr_capacity_);
}

template <typename Type, typename Allocator>
typename Deque<Type, Allocator>::Index Deque<Type, Allocator>::get_index(
    size_t index) const {
  if (index < kMaxSize - front_.ind) {
    return {front_.ptr, front_.ind + index};
  }
  size_t temp = index - (kMaxSize - front_.ind);
  return {front_.ptr + 1 + temp / kMaxSize, temp % kMaxSize};
}

template <typename Type, typename Allocator>
template <bool IsConst>
class Deque<Type, Allocator>::Iterator {
 public:
  friend Deque;
  using value_type = std::conditional_t<IsConst, const Type, Type>;
  using iterator_category = std::random_access_iterator_tag;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = size_t;

  Iterator(Type** arr, size_t ind) : arr_(arr), ind_(ind) {}

  operator Iterator<true>() { return Iterator<true>(arr_); }

  Iterator& operator++();

  Iterator operator++(int);

  Iterator& operator+=(int value);

  Iterator operator+(int value) const;

  Iterator& operator--();

  Iterator operator--(int);

  Iterator& operator-=(int value);

  Iterator operator-(int value) const;

  pointer operator->() const { return *arr_ + ind_; }

  reference operator*() const { return (*arr_)[ind_]; }

  template <bool OtherIsConst>
  bool operator<(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                     other_iterator) const;

  template <bool OtherIsConst>
  bool operator>(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                     other_iterator) const;

  template <bool OtherIsConst>
  bool operator<=(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                      other_iterator) const;

  template <bool OtherIsConst>
  bool operator>=(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                      other_iterator) const;

  template <bool OtherIsConst>
  bool operator==(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                      other_iterator) const;

  template <bool OtherIsConst>
  bool operator!=(const Deque<Type, Allocator>::Iterator<OtherIsConst>&
                      other_iterator) const;

  size_t operator-(
      const Deque<Type, Allocator>::Iterator<IsConst>& other_iterator);

 private:
  Type** arr_;
  size_t ind_;
};

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>&
Deque<Type, Allocator>::Iterator<IsConst>::operator++() {
  return *this += 1;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>
Deque<Type, Allocator>::Iterator<IsConst>::operator++(int) {
  auto copy = *this;
  ++*this;
  return copy;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>&
Deque<Type, Allocator>::Iterator<IsConst>::operator+=(int value) {
  if (value > 0) {
    auto new_value = static_cast<size_t>(value);
    if (new_value + ind_ < kMaxSize) {
      ind_ += new_value;
      return *this;
    }
    size_t temp = new_value - (kMaxSize - ind_);
    arr_ += 1 + temp / kMaxSize;
    ind_ = temp % kMaxSize;
  } else if (value < 0) {
    value *= -1;
    auto new_value = static_cast<size_t>(value);
    if (ind_ >= new_value) {
      ind_ -= new_value;
      return *this;
    }
    new_value -= ind_;
    size_t diff_in_external = 0;
    if ((new_value & (kMaxSize - 1)) != 0) {
      ++diff_in_external;
    }
    diff_in_external += new_value / kMaxSize;
    ind_ = kMaxSize - 1 - (new_value - 1) % kMaxSize;
    arr_ -= diff_in_external;
  }
  return *this;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>
Deque<Type, Allocator>::Iterator<IsConst>::operator+(int value) const {
  Iterator<IsConst> copy(*this);
  copy += value;
  return copy;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>&
Deque<Type, Allocator>::Iterator<IsConst>::operator--() {
  return *this -= 1;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>
Deque<Type, Allocator>::Iterator<IsConst>::operator--(int) {
  auto copy = *this;
  --*this;
  return copy;
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>&
Deque<Type, Allocator>::Iterator<IsConst>::operator-=(int value) {
  return *this += (-value);
}

template <typename Type, typename Allocator>
template <bool IsConst>
typename Deque<Type, Allocator>::template Iterator<IsConst>
Deque<Type, Allocator>::Iterator<IsConst>::operator-(int value) const {
  Iterator<IsConst> copy(*this);
  copy -= value;
  return copy;
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator<(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return arr_ < other_iterator.arr_ ||
         (arr_ == other_iterator.arr_ && ind_ < other_iterator.ind_);
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator>(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return !(*this <= other_iterator);
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator<=(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return *this < other_iterator || *this == other_iterator;
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator>=(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return *this > other_iterator || *this == other_iterator;
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator==(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return arr_ == other_iterator.arr_ && ind_ == other_iterator.ind_;
}

template <typename Type, typename Allocator>
template <bool IsConst>
template <bool OtherIsConst>
bool Deque<Type, Allocator>::Iterator<IsConst>::operator!=(
    const Deque<Type, Allocator>::Iterator<OtherIsConst>& other_iterator)
    const {
  return !(*this == other_iterator);
}

template <typename Type, typename Allocator>
template <bool IsConst>
size_t Deque<Type, Allocator>::Iterator<IsConst>::operator-(
    const Deque<Type, Allocator>::Iterator<IsConst>& other_iterator) {
  return kMaxSize * (arr_ - other_iterator.arr_) + ind_ - other_iterator.ind_;
}

template <typename Type, typename Allocator>
template <bool IsConst>
void Deque<Type, Allocator>::erase(Deque::Iterator<IsConst> iter) {
  auto copy = iter;
  while (copy != end() - 1) {
    std::swap(*copy, *(copy + 1));
    ++copy;
  }
  pop_back();
}

template <typename Type, typename Allocator>
bool Deque<Type, Allocator>::empty() {
  return size_ == 0;
}
