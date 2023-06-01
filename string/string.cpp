#include "string.hpp"

//
// Created by Daniel on 11.11.2022.
//

uint64_t CapacityCheck(uint64_t value) {
  if (value == 0) {
    return 0;
  }
  uint64_t capacity = 1;
  while (capacity < value) {
    capacity *= 2;
  }
  return capacity;
}

void String::Realoc() {
  if (capacity_ == 0) {
    ++capacity_;
  } else {
    capacity_ *= 2;
  }
  if (str_ != nullptr) {
    char* new_string = new char[capacity_ + 1];
    memcpy(new_string, str_, size_);
    char* memory = str_;
    str_ = new_string;
    delete[] memory;
  } else {
    str_ = new char[capacity_ + 1];
  }
}

void String::Realoc(uint64_t new_size) {
  capacity_ = CapacityCheck(new_size);
  if (str_ != nullptr) {
    char* new_string = new char[capacity_ + 1];
    memcpy(new_string, str_, size_);
    char* memory = str_;
    str_ = new_string;
    delete[] memory;
  } else {
    str_ = new char[capacity_ + 1];
  }
}

bool operator<(const String& first_string, const String& second_string) {
  if (first_string.size_ < second_string.size_) {
    return true;
  }
  if (first_string.size_ == second_string.size_) {
    for (uint64_t index = 0; index < first_string.size_; ++index) {
      if (first_string.str_[index] < second_string.str_[index]) {
        return true;
      }
      if (first_string.str_[index] > second_string.str_[index]) {
        return false;
      }
    }
  }
  return false;
}

bool operator>(const String& first_string, const String& second_string) {
  return second_string < first_string;
}

bool operator<=(const String& first_string, const String& second_string) {
  if (first_string.size_ < second_string.size_) {
    return true;
  }
  if (first_string.size_ > second_string.size_) {
    return false;
  }
  if (first_string.size_ == second_string.size_) {
    for (uint64_t index = 0; index < first_string.size_; ++index) {
      if (first_string.str_[index] < second_string.str_[index]) {
        return true;
      }
      if (first_string.str_[index] > second_string.str_[index]) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool operator>=(const String& first_string, const String& second_string) {
  return second_string <= first_string;
}

bool operator==(const String& first_string, const String& second_string) {
  return !(second_string < first_string || first_string < second_string);
}

bool operator!=(const String& first_string, const String& second_string) {
  return !(first_string == second_string);
}

std::ostream& operator<<(std::ostream& output, const String& some_string) {
  for (uint64_t index = 0; index < some_string.size_; ++index) {
    output << some_string.str_[index];
  }
  return output;
}

std::istream& operator>>(std::istream& input, String& some_string) {
  char symbol;
  some_string.Clear();
  while (input.get(symbol) && (std::isspace(symbol) == 0)) {
    some_string.PushBack(symbol);
  }
  return input;
}

String::String() = default;

String::String(uint64_t size, char character)
    : size_(size),
      capacity_(CapacityCheck(size_)),
      str_(new char[capacity_ + 1]) {
  memset(str_, character, size_);
  str_[size_] = '\0';
}

String::String(const char* some_string)
    : size_(strlen(some_string)),
      capacity_(CapacityCheck(size_)),
      str_(new char[capacity_ + 1]) {
  memcpy(str_, some_string, size_);
  str_[size_] = '\0';
}

String::String(const String& some_string)
    : capacity_(some_string.capacity_),
      size_(some_string.size_),
      str_(new char[capacity_ + 1]) {
  memcpy(str_, some_string.str_, size_);
  str_[size_] = '\0';
}

String& String::operator=(String some_string) {
  Swap(some_string);
  return *this;
}

void String::Resize(uint64_t new_size) {
  if (new_size <= capacity_) {
    size_ = new_size;
  } else {
    Realoc(new_size);
    size_ = new_size;
  }
}

void String::Resize(uint64_t new_size, char character) {
  uint64_t last_size = size_;
  Resize(new_size);
  for (uint64_t index = last_size; index < new_size; ++index) {
    str_[index] = character;
  }
  str_[size_] = '\0';
}

void String::PushBack(char character) {
  if (capacity_ - size_ < 1) {
    Realoc(size_ + 1);
  }
  str_[size_] = character;
  ++size_;
  str_[size_] = '\0';
}

void String::Reserve(uint64_t new_cap) {
  if (new_cap > capacity_) {
    capacity_ = new_cap;
  }
}

void String::Swap(String& some_string) {
  std::swap(size_, some_string.size_);
  std::swap(capacity_, some_string.capacity_);
  std::swap(str_, some_string.str_);
}

uint64_t String::Size() const { return size_; }

uint64_t String::Capacity() const { return capacity_; }

char* String::Data() { return str_; }

void String::Clear() { size_ = 0; }

void String::PopBack() {
  if (size_ != 0) {
    str_[size_ - 1] = '\0';
    --size_;
  }
}

String& String::operator+=(const String& some_string) {
  if (capacity_ < size_ + some_string.size_) {
    Realoc(some_string.size_ + size_);
  }
  for (uint64_t index = size_; index < size_ + some_string.size_; ++index) {
    str_[index] = some_string.str_[index - size_];
  }
  size_ += some_string.size_;
  str_[size_] = '\0';
  return *this;
}

String String::operator+(const String& some_string) const {
  String copy = *this;
  copy += some_string;
  return copy;
}

const char& String::operator[](uint64_t index) const { return str_[index]; }

char& String::operator[](uint64_t index) { return str_[index]; }

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
  }
}

bool String::Empty() const { return size_ == 0; }

char& String::Front() { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

const char& String::Front() const { return str_[0]; }

const char& String::Back() const { return str_[size_ - 1]; }

std::vector<String> String::Split(const String& delim) {
  std::vector<String> result;
  String substr;
  std::vector<uint64_t> vec = this->Find(delim);
  for (uint64_t index = 0; index < vec.size() - 1; ++index) {
    char* stroka = new char[vec[index + 1] - vec[index] - delim.size_ + 1];
    memcpy(stroka, str_ + vec[index],
           vec[index + 1] - vec[index] - delim.size_);
    stroka[vec[index + 1] - vec[index] - delim.size_] = '\0';
    result.push_back(stroka);
    delete[] stroka;
  }
  return result;
}

String String::Join(const std::vector<String>& strings) const {
  String result("");
  if (!strings.empty()) {
    for (uint64_t index = 0; index < strings.size() - 1; ++index) {
      result += (strings[index] + *this);
    }
    result += strings.back();
  }
  return result;
}

std::vector<uint64_t> String::Find(const String& some_string) {
  std::vector<uint64_t> result(1, 0);
  char* pointer_to_position;
  while ((pointer_to_position =
              strstr(str_ + result.back(), some_string.str_)) != nullptr) {
    result.push_back((pointer_to_position - str_) + some_string.size_);
  }
  result.push_back(capacity_ + some_string.size_);
  return result;
}

String::~String() { delete[] str_; }

const char* String::Data() const { return str_; }

String& String::operator*=(uint64_t number) {
  String result = *this;
  for (uint64_t index = 0; index < number - 1; ++index) {
    *this += result;
  }
  return *this;
}

String operator*(const String& k_some_string, uint64_t number) {
  String result;
  for (uint64_t index = 0; index < number; ++index) {
    result += k_some_string;
  }
  return result;
}
