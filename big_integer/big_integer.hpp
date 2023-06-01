//
// Created by Daniel on 21.11.2022.
//

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class BigInt {
 private:
  int sign_ = 1;
  std::vector<int64_t> digits_;

 public:
  BigInt();
  BigInt(std::string);
  BigInt(int64_t some_number);
  void FillCopyMinus(BigInt& copy, const BigInt& some_big_number) const;
  void FillCopyPlus(BigInt& copy, const BigInt& some_big_number) const;
  BigInt operator-() const;
  BigInt operator-(const BigInt&) const;
  BigInt operator+(const BigInt&) const;
  BigInt operator*(const BigInt&) const;
  BigInt operator/(const BigInt&) const;
  friend bool operator>=(const BigInt& first_big_value,
                         const BigInt& second_big_value);
  friend bool operator==(const BigInt& first_big_value,
                         const BigInt& second_big_value);
  friend bool operator<=(const BigInt& first_big_value,
                         const BigInt& second_big_value);
  friend bool operator<(const BigInt& first_big_value,
                        const BigInt& second_big_value);
  friend bool operator>(const BigInt& first_big_value,
                        const BigInt& second_big_value);
  friend bool operator!=(const BigInt& first_big_value,
                         const BigInt& second_big_value);
  friend std::istream& operator>>(std::istream& input, BigInt& number);
  friend std::ostream& operator<<(std::ostream& output, const BigInt& number);
  BigInt operator%(const BigInt& some_big_value) const;
  BigInt& operator+=(const BigInt& some_big_value);
  BigInt& operator-=(const BigInt& some_big_value);

  BigInt& operator*=(const BigInt& some_big_value);

  BigInt& operator/=(const BigInt& some_big_value);

  BigInt& operator%=(const BigInt& some_big_value);
  BigInt& operator++();

  BigInt& operator--();

  BigInt operator++(int);

  BigInt operator--(int);
};
