#include <iostream>
#include <memory>

#pragma once

template <typename T, typename Alloc = std::allocator<T>>
class List {
 public:
  using value_type = T;
  using allocator_type = Alloc;

  List();

  List(size_t amount, const value_type& value);

  List(size_t amount);

  List(allocator_type alloc);

  List(size_t amount, allocator_type alloc);

  List(size_t amount, const value_type& value, allocator_type alloc);

  List(std::initializer_list<value_type> init,
       const allocator_type& alloc = Alloc());

  List(const List& other_list);

  List(List&& other) noexcept;

  List& operator=(const List& other_list);

  List& operator=(List&& other) noexcept;

  size_t size() const;

  bool empty() const;

  template <typename... Args>
  void emplace_back(Args&&... args);

  template <typename... Args>
  void emplace_front(Args&&... args);

  void push_back(const value_type& value);

  void push_back(value_type&& value);

  void push_front(const value_type& value);

  void push_front(value_type&& value);

  void pop_back();

  void pop_front();

  value_type& back();

  const value_type& back() const;

  value_type& front();

  const value_type& front() const;

  template <bool IsConst>
  class Iterator;

  template <bool IsConst>
  using rever_iterator = std::reverse_iterator<Iterator<IsConst>>;

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = rever_iterator<false>;
  using const_reverse_iterator = rever_iterator<true>;

  iterator begin() {
    return iterator(reinterpret_cast<Node*>(fake_node_->next));
  }

  const_iterator begin() const {
    return const_iterator(reinterpret_cast<Node*>(fake_node_->next));
  }

  const_iterator cbegin() const {
    return const_iterator(reinterpret_cast<Node*>(fake_node_->next));
  }

  iterator end() { return iterator(reinterpret_cast<Node*>(fake_node_)); }

  const_iterator end() const {
    return const_iterator(reinterpret_cast<Node*>(fake_node_));
  }

  const_iterator cend() const {
    return const_iterator(reinterpret_cast<Node*>(fake_node_));
  }

  reverse_iterator rbegin() {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_));
  }

  const_reverse_iterator rbegin() const {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_));
  }

  const_reverse_iterator crbegin() const {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_));
  }

  reverse_iterator rend() {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_->previous));
  }

  const_reverse_iterator rend() const {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_->previous));
  }

  const_reverse_iterator crend() const {
    return reverse_iterator(reinterpret_cast<Node*>(fake_node_->previous));
  }

  const allocator_type& get_allocator();

  template <bool IsConst, typename... Args>
  void emplace(const Iterator<IsConst>& pos, Args&&... args);

  template <bool IsConst>
  void erase(const Iterator<IsConst>& pos);

  ~List();

 private:
  struct FakeNode {
    FakeNode() = default;

    FakeNode* next;
    FakeNode* previous;
  };

  struct Node : FakeNode {
    Node() = default;

    Node(const value_type& value) : value(value) {}

    Node(value_type&& value) noexcept : value(std::move(value)) {}

    template <typename... Args>
    Node(Args&&... args) : value(std::forward<Args>(args)...) {}

    value_type value;
  };

  using allocator_traits = std::allocator_traits<allocator_type>;
  using rebind_allocator =
      typename allocator_traits::template rebind_alloc<Node>;
  using rebind_allocator_traits = std::allocator_traits<rebind_allocator>;

  size_t size_;
  rebind_allocator alloc_;
  allocator_type true_alloc_;

  FakeNode* fake_node_;
};

template <typename T, typename Alloc>
List<T, Alloc>::List(std::initializer_list<value_type> init,
                     const allocator_type& alloc)
    : size_(0),
      alloc_(alloc),
      true_alloc_(alloc),
      fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  FakeNode* prev = fake_node_;
  try {
    for (const value_type& val : init) {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      rebind_allocator_traits::construct(alloc_, new_node, val);
      new_node->previous = prev;
      prev->next = new_node;
      prev = new_node;
      ++size_;
    }
    prev->next = fake_node_;
    fake_node_->previous = prev;
  } catch (...) {
    prev->next = fake_node_;
    fake_node_->previous = prev;
    while (size_ > 0) {
      pop_back();
    }
    rebind_allocator_traits::deallocate(alloc_,
                                        reinterpret_cast<Node*>(fake_node_), 1);
    throw;
  }
}

template <typename T, typename Alloc>
bool List<T, Alloc>::empty() const {
  return (size() == 0);
}

template <typename T, typename Alloc>
List<T, Alloc>::List()
    : size_(0), fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->previous = fake_node_;
  fake_node_->next = fake_node_;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(size_t amount, const value_type& value)
    : size_(0), fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->next = fake_node_;
  fake_node_->previous = fake_node_;
  FakeNode* prev = fake_node_;
  for (size_t count = 0; count < amount; ++count) {
    try {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      rebind_allocator_traits::construct(alloc_, new_node, value);
      new_node->previous = prev;
      prev->next = new_node;
      prev = new_node;
      ++size_;
    } catch (...) {
      prev->next = fake_node_;
      fake_node_->previous = prev;
      while (size_ > 0) {
        pop_back();
      }
      rebind_allocator_traits::deallocate(
          alloc_, reinterpret_cast<Node*>(fake_node_), 1);
      throw;
    }
  }
  prev->next = fake_node_;
  fake_node_->previous = prev;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(size_t amount)
    : size_(0), fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->next = fake_node_;
  fake_node_->previous = fake_node_;
  FakeNode* prev = fake_node_;
  for (size_t count = 0; count < amount; ++count) {
    try {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      try {
        rebind_allocator_traits::construct(alloc_, new_node);
      } catch (...) {
        rebind_allocator_traits::deallocate(alloc_, new_node, 1);
        throw;
      }
      new_node->previous = prev;
      prev->next = new_node;
      prev = new_node;
      new_node->next = fake_node_;
      ++size_;
    } catch (...) {
      prev->next = fake_node_;
      fake_node_->previous = prev;
      while (size_ > 0) {
        pop_back();
      }
      rebind_allocator_traits::deallocate(
          alloc_, reinterpret_cast<Node*>(fake_node_), 1);
      throw;
    }
  }
  prev->next = fake_node_;
  fake_node_->previous = prev;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(allocator_type alloc)
    : size_(0),
      alloc_(alloc),
      true_alloc_(alloc),
      fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->previous = fake_node_;
  fake_node_->next = fake_node_;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(size_t amount, Alloc alloc)
    : size_(0),
      alloc_(alloc),
      true_alloc_(alloc),
      fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->next = fake_node_;
  fake_node_->previous = fake_node_;
  FakeNode* prev = fake_node_;
  try {
    for (size_t count = 0; count < amount; ++count) {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      rebind_allocator_traits::construct(alloc_, new_node);
      new_node->previous = prev;
      prev->next = new_node;
      prev = new_node;
      ++size_;
    }
  } catch (...) {
    prev->next = fake_node_;
    fake_node_->previous = prev;
    while (size_ > 0) {
      pop_back();
    }
    rebind_allocator_traits::deallocate(alloc_,
                                        reinterpret_cast<Node*>(fake_node_), 1);
    throw;
  }
  prev->next = fake_node_;
  fake_node_->previous = prev;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(size_t amount, const value_type& value,
                     allocator_type alloc)
    : size_(0),
      alloc_(alloc),
      true_alloc_(alloc),
      fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  fake_node_->next = fake_node_;
  fake_node_->previous = fake_node_;
  FakeNode* prev = fake_node_;
  for (size_t count = 0; count < amount; ++count) {
    try {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      rebind_allocator_traits::construct(alloc_, new_node, value);
      new_node->previous = prev;
      prev->next = new_node;
      new_node->next = fake_node_;
      prev = new_node;
      ++size_;
    } catch (...) {
      prev->next = fake_node_;
      fake_node_->previous = prev;
      while (size_ > 0) {
        pop_back();
      }
      rebind_allocator_traits::deallocate(
          alloc_, reinterpret_cast<Node*>(fake_node_), 1);
      throw;
    }
  }
  prev->next = fake_node_;
  fake_node_->previous = prev;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(const List& other_list)
    : size_(0),
      alloc_(rebind_allocator_traits::select_on_container_copy_construction(
          other_list.alloc_)),
      true_alloc_(
          rebind_allocator_traits::select_on_container_copy_construction(
              other_list.true_alloc_)) {
  fake_node_ = rebind_allocator_traits::allocate(alloc_, 1);
  fake_node_->next = fake_node_;
  fake_node_->previous = fake_node_;
  FakeNode* prev = fake_node_;
  for (auto& iter : other_list) {
    try {
      Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
      try {
        rebind_allocator_traits::construct(alloc_, new_node, iter);
      } catch (...) {
        rebind_allocator_traits::deallocate(alloc_, new_node, 1);
        throw;
      }
      new_node->previous = prev;
      prev->next = new_node;
      prev = new_node;
      prev->next = fake_node_;
      fake_node_->previous = prev;
      ++size_;
    } catch (...) {
      prev->next = fake_node_;
      fake_node_->previous = prev;
      while (size_ > 0) {
        pop_back();
      }
      rebind_allocator_traits::deallocate(
          alloc_, reinterpret_cast<Node*>(fake_node_), 1);
      throw;
    }
  }
  prev->next = fake_node_;
  fake_node_->previous = prev;
}

template <typename T, typename Alloc>
List<T, Alloc>::List(List&& other_list) noexcept
    : size_(0), fake_node_(rebind_allocator_traits::allocate(alloc_, 1)) {
  std::swap(size_, other_list.size_);
  std::swap(fake_node_, other_list.fake_node_);
  std::swap(alloc_, other_list.alloc_);
  std::swap(true_alloc_, other_list.true_alloc_);
}

template <typename T, typename Alloc>
List<T, Alloc>& List<T, Alloc>::operator=(const List& other_list) {
  if (rebind_allocator_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = other_list.alloc_;
    true_alloc_ = other_list.true_alloc_;
  }
  size_t count = 0;
  auto iter = begin();
  for (auto& other_it : other_list) {
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
      while (count > 0) {
        pop_back();
        --count;
      }
      rebind_allocator_traits::deallocate(
          alloc_, reinterpret_cast<Node*>(fake_node_), 1);
      throw;
    }
  }
  while (size_ > other_list.size_) {
    pop_back();
  }
  size_ = other_list.size_;
  return *this;
}

template <typename T, typename Alloc>
List<T, Alloc>& List<T, Alloc>::operator=(List&& other_list) noexcept {
  std::swap(size_, other_list.size_);
  std::swap(fake_node_, other_list.fake_node_);
  std::swap(alloc_, other_list.alloc_);
  std::swap(true_alloc_, other_list.true_alloc_);
  return *this;
}

template <typename T, typename Alloc>
size_t List<T, Alloc>::size() const {
  return size_;
}

template <typename T, typename Alloc>
template <typename... Args>
void List<T, Alloc>::emplace_back(Args&&... args) {
  try {
    emplace(end(), std::forward<Args>(args)...);
  } catch (...) {
    pop_back();
    throw;
  }
}

template <typename T, typename Alloc>
template <typename... Args>
void List<T, Alloc>::emplace_front(Args&&... args) {
  try {
    emplace(begin(), std::forward<Args>(args)...);
  } catch (...) {
    pop_back();
    throw;
  }
}

template <typename T, typename Alloc>
void List<T, Alloc>::push_back(const T& value) {
  try {
    emplace_back(value);
  } catch (...) {
    pop_back();
    throw;
  }
}

template <typename T, typename Alloc>
void List<T, Alloc>::push_back(T&& value) {
  try {
    emplace_back(std::move(value));
  } catch (...) {
    pop_back();

    throw;
  }
}

template <typename T, typename Alloc>
void List<T, Alloc>::push_front(const T& value) {
  try {
    emplace_front(value);
  } catch (...) {
    pop_back();
    throw;
  }
}

template <typename T, typename Alloc>
void List<T, Alloc>::push_front(T&& value) {
  try {
    emplace_front(std::move(value));
  } catch (...) {
    pop_back();
    throw;
  }
}

template <typename T, typename Alloc>
void List<T, Alloc>::pop_back() {
  Node* tmp = reinterpret_cast<Node*>(fake_node_->previous);
  fake_node_->previous->previous->next = fake_node_;
  fake_node_->previous = fake_node_->previous->previous;
  rebind_allocator_traits::destroy(alloc_, tmp);
  rebind_allocator_traits::deallocate(alloc_, tmp, 1);
  --size_;
}

template <typename T, typename Alloc>
void List<T, Alloc>::pop_front() {
  Node* tmp = reinterpret_cast<Node*>(fake_node_->next);
  fake_node_->next->next->previous = fake_node_;
  fake_node_->next = fake_node_->next->next;
  rebind_allocator_traits::destroy(alloc_, tmp);
  rebind_allocator_traits::deallocate(alloc_, tmp, 1);
  --size_;
}

template <typename T, typename Alloc>
T& List<T, Alloc>::back() {
  return reinterpret_cast<Node*>(fake_node_->previous)->value;
}

template <typename T, typename Alloc>
const T& List<T, Alloc>::back() const {
  return reinterpret_cast<Node*>(fake_node_->previous)->value;
}

template <typename T, typename Alloc>
T& List<T, Alloc>::front() {
  return reinterpret_cast<Node*>(fake_node_->next)->value;
}

template <typename T, typename Alloc>
const T& List<T, Alloc>::front() const {
  return reinterpret_cast<Node*>(fake_node_->next)->value;
}

template <typename T, typename Alloc>
template <bool IsConst>
class List<T, Alloc>::Iterator {
 public:
  friend List;
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = std::conditional_t<IsConst, const T, T>;
  using reference = value_type&;
  using pointer = value_type*;

  Iterator(Node* node) : node_(node) {}

  operator Iterator<true>() { return Iterator<true>(node_); }

  Iterator& operator++();

  Iterator& operator--();

  Iterator operator++(int);

  Iterator operator--(int);

  pointer operator->() const { return &node_->value; }

  reference operator*() { return node_->value; }

  template <bool OtherIsConst>
  bool operator==(
      const List<T, Alloc>::Iterator<OtherIsConst>& other_iterator) const;

  template <bool OtherIsConst>
  bool operator!=(
      const List<T, Alloc>::Iterator<OtherIsConst>& other_iterator) const;

  difference_type operator-(
      const List<T, Alloc>::Iterator<IsConst>& other_iterator);

 private:
  Node* node_;
};

template <typename T, typename Alloc>
template <bool IsConst>
typename List<T, Alloc>::template Iterator<IsConst>&
List<T, Alloc>::Iterator<IsConst>::operator++() {
  node_ = reinterpret_cast<Node*>(node_->next);
  return *this;
}

template <typename T, typename Alloc>
template <bool IsConst>
typename List<T, Alloc>::template Iterator<IsConst>&
List<T, Alloc>::Iterator<IsConst>::operator--() {
  node_ = reinterpret_cast<Node*>(node_->previous);
  return *this;
}

template <typename T, typename Alloc>
template <bool IsConst>
typename List<T, Alloc>::template Iterator<IsConst>
List<T, Alloc>::Iterator<IsConst>::operator++(int) {
  Iterator copy = *this;
  ++*this;
  return copy;
}

template <typename T, typename Alloc>
template <bool IsConst>
typename List<T, Alloc>::template Iterator<IsConst>
List<T, Alloc>::Iterator<IsConst>::operator--(int) {
  Iterator copy = *this;
  --*this;
  return copy;
}

template <typename T, typename Alloc>
template <bool IsConst>
template <bool OtherIsConst>
bool List<T, Alloc>::Iterator<IsConst>::operator==(
    const typename List<T, Alloc>::template Iterator<OtherIsConst>&
        other_iterator) const {
  return node_ == other_iterator.node_;
}

template <typename T, typename Alloc>
template <bool IsConst>
template <bool OtherIsConst>
bool List<T, Alloc>::Iterator<IsConst>::operator!=(
    const typename List<T, Alloc>::template Iterator<OtherIsConst>&
        other_iterator) const {
  return node_ != other_iterator.node_;
}

template <typename T, typename Alloc>
List<T, Alloc>::~List() {
  while (size_ > 0) {
    pop_back();
  }
  rebind_allocator_traits::deallocate(alloc_,
                                      reinterpret_cast<Node*>(fake_node_), 1);
}

template <typename T, typename Alloc>
template <bool IsConst>
typename List<T, Alloc>::template Iterator<IsConst>::difference_type
List<T, Alloc>::Iterator<IsConst>::operator-(
    const List<T, Alloc>::Iterator<IsConst>& other_iterator) {
  difference_type count = 0;
  while (other_iterator != *this) {
    ++count;
    ++other_iterator;
  }
  return count;
}

template <typename T, typename Alloc>
const Alloc& List<T, Alloc>::get_allocator() {
  return true_alloc_;
}

template <typename T, typename Alloc>
template <bool IsConst, typename... Args>
void List<T, Alloc>::emplace(const List::Iterator<IsConst>& pos,
                             Args&&... args) {
  Node* new_node = rebind_allocator_traits::allocate(alloc_, 1);
  try {
    rebind_allocator_traits::construct(alloc_, new_node,
                                       std::forward<Args>(args)...);
  } catch (...) {
    pop_back();
    throw;
  }
  pos.node_->previous->next = new_node;
  new_node->previous = pos.node_->previous;
  pos.node_->previous = new_node;
  new_node->next = pos.node_;
  ++size_;
}

template <typename T, typename Alloc>
template <bool IsConst>
void List<T, Alloc>::erase(const Iterator<IsConst>& pos) {
  pos.node_->previous->next = pos.node_->next;
  pos.node_->next->previous = pos.node_->previous;
  rebind_allocator_traits::destroy(alloc_, pos.node_);
  rebind_allocator_traits::deallocate(alloc_, pos.node_, 1);
  --size_;
}
