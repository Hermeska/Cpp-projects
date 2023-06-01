#pragma once
#include <cstring>
#include <iostream>
#include <vector>
uint64_t CapacityCheck(uint64_t value);
class String {
 private:
  uint64_t size_ = 0;
  uint64_t capacity_ = 0;
  char* str_ = nullptr;
  void Realoc();
  void Realoc(uint64_t new_size);
  friend bool operator<(const String& first_string,
                        const String& second_string);
  friend bool operator<=(const String& first_string,
                         const String& second_string);
  friend std ::ostream& operator<<(std::ostream& output,
                                   const String& some_string);
  friend std::istream& operator>>(std::istream& input, String& some_string);

 public:
  String();
  String(uint64_t size, char character);
  String(const char* some_string);
  String(const String& some_string);
  String& operator=(String some_string);
  void Resize(uint64_t new_size);
  void Resize(uint64_t new_size, char character);
  void PushBack(char character);
  void Reserve(uint64_t new_cap);
  void Swap(String& some_string);
  uint64_t Size() const;
  uint64_t Capacity() const;
  char* Data();
  const char* Data() const;
  void Clear();
  void PopBack();
  String& operator+=(const String& some_string);
  String& operator*=(uint64_t number);
  String operator+(const String& some_string) const;
  const char& operator[](uint64_t index) const;
  char& operator[](uint64_t index);
  void ShrinkToFit();
  bool Empty() const;
  char& Front();
  char& Back();
  const char& Front() const;
  const char& Back() const;
  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings) const;
  std::vector<uint64_t> Find(const String& some_string);

  ~String();
};
String operator*(const String& k_some_string, uint64_t number);
bool operator<(const String& first_string, const String& second_string);
bool operator<=(const String& first_string, const String& second_string);
bool operator>=(const String& first_string, const String& second_string);
bool operator>(const String& first_string, const String& second_string);
bool operator==(const String& first_string, const String& second_string);
bool operator!=(const String& first_string, const String& second_string);
std::ostream& operator<<(std::ostream& output, const String& some_string);
std::istream& operator>>(std::ifstream& input, String& some_string);
