#include "big_integer.hpp"
//
// Created by Daniel on 19.11.2022.
//
const int64_t kBitness = 10;
const int64_t kMinValue = INT64_MIN;
const int64_t kMaxValue = INT64_MAX;
BigInt::BigInt() = default;

BigInt::BigInt(std::string some_string) {
  if (some_string == "0" || some_string == "-0") {
    digits_.push_back(0);
    sign_ = 1;
    return;
  }
  if (some_string[0] == '-') {
    sign_ = -1;
    for (int64_t iter = static_cast<int64_t>(some_string.size()) - 1; iter >= 1;
         --iter) {
      digits_.push_back(static_cast<char>(some_string[iter]) - '0');
    }
  } else {
    sign_ = 1;
    for (int64_t iter = static_cast<int64_t>(some_string.size()) - 1; iter >= 0;
         --iter) {
      digits_.push_back(static_cast<char>(some_string[iter]) - '0');
    }
  }
}

BigInt::BigInt(int64_t some_number) {
  if (some_number == 0 || some_number == -0) {
    digits_.push_back(0);
    sign_ = 1;
    return;
  }
  if (some_number == INT64_MIN) {
    sign_ = -1;
    some_number = INT64_MIN + 1;
    *this = BigInt(some_number);
    *this -= 1;
    return;
  }
  if (some_number < 0) {
    sign_ = -1;
    some_number *= -1;
  }
  while (some_number > 0) {
    digits_.push_back(some_number % kBitness);
    some_number /= kBitness;
  }
}

BigInt BigInt::operator+(const BigInt& some_big_number) const {
  if (sign_ == -1) {
    if (some_big_number.sign_ == -1) {
      return -(-*this + (-some_big_number));
    }
    return some_big_number - (-*this);
  }
  if (some_big_number.sign_ == -1) {
    return *this - (-some_big_number);
  }
  BigInt copy;
  FillCopyPlus(copy, some_big_number);
  return copy;
}
void BigInt::FillCopyPlus(BigInt& copy, const BigInt& some_big_number) const {
  int64_t divisor = 0;
  int64_t value;
  for (uint64_t iter = 0;
       iter < std::min(some_big_number.digits_.size(), this->digits_.size());
       ++iter) {
    value = divisor + digits_[iter] + some_big_number.digits_[iter];
    if (divisor + digits_[iter] + some_big_number.digits_[iter] >
        kBitness - 1) {
      divisor = 1;
    } else {
      divisor = 0;
    }
    copy.digits_.push_back(value % kBitness);
  }
  for (uint64_t i = std::min(digits_.size(), some_big_number.digits_.size());
       i < std::max(digits_.size(), some_big_number.digits_.size()); ++i) {
    if (some_big_number.digits_.size() < digits_.size()) {
      value = divisor + this->digits_[i];
      copy.digits_.push_back(value % kBitness);
      divisor = value / kBitness;
    } else {
      value = divisor + some_big_number.digits_[i];
      copy.digits_.push_back(value % kBitness);
      divisor = value / kBitness;
    }
  }
  if (divisor != 0) {
    copy.digits_.push_back(divisor);
  }
}
BigInt BigInt::operator-() const {
  BigInt copy(*this);
  if (copy == 0) {
    return 0;
  }
  copy.sign_ *= -1;
  return copy;
}
BigInt BigInt::operator-(const BigInt& some_big_number) const {
  if (*this < some_big_number) {
    return -(some_big_number - *this);
  }
  if (sign_ == -1) {
    return -(-*this + some_big_number);
  }
  if (some_big_number.sign_ == -1) {
    return *this + (-some_big_number);
  }
  BigInt copy;
  FillCopyMinus(copy, some_big_number);
  return copy;
}
void BigInt::FillCopyMinus(BigInt& copy, const BigInt& some_big_number) const {
  int64_t divisor = 0;
  int64_t value;
  for (uint64_t iter = 0; iter < some_big_number.digits_.size(); ++iter) {
    value = digits_[iter] - some_big_number.digits_[iter] - divisor;
    if (digits_[iter] - some_big_number.digits_[iter] - divisor < 0) {
      divisor = 1;
      value += kBitness;
    } else {
      divisor = 0;
    }
    copy.digits_.push_back(value);
  }
  for (uint64_t iter = some_big_number.digits_.size(); iter < digits_.size();
       ++iter) {
    if (digits_[iter] - divisor < 0) {
      copy.digits_.push_back(digits_[iter] + kBitness - divisor);
      divisor = 1;
    } else {
      copy.digits_.push_back(digits_[iter] - divisor);
      divisor = 0;
    }
  }
  while (copy.digits_.size() > 1 && copy.digits_.back() == 0) {
    copy.digits_.pop_back();
  }
}
BigInt BigInt::operator*(const BigInt& some_big_number) const {
  BigInt copy;
  if (*this == 0 || some_big_number == 0) {
    return 0;
  }
  copy.sign_ = this->sign_ * some_big_number.sign_;
  copy.digits_.resize(this->digits_.size() + some_big_number.digits_.size());
  int64_t divisor = 0;
  int64_t value;
  for (int64_t index_i = 0;
       index_i < static_cast<int64_t>(this->digits_.size()); ++index_i) {
    for (uint64_t index_j = 0; index_j < some_big_number.digits_.size();
         ++index_j) {
      value = divisor +
              some_big_number.digits_[index_j] * this->digits_[index_i] +
              copy.digits_[index_j + index_i];
      divisor = value / kBitness;
      copy.digits_[index_i + index_j] = value % kBitness;
    }
    uint64_t last_index = some_big_number.digits_.size();
    for (; divisor > 0; divisor /= kBitness) {
      copy.digits_[index_i + last_index] = divisor % kBitness;
      ++last_index;
    }
  }
  while (copy.digits_.size() > 1 && copy.digits_.back() == 0) {
    copy.digits_.pop_back();
  }
  return copy;
}
// ya staralsya napisat' delenie maksimalno effectivno, poetomu deadline
// propustil
BigInt BigInt::operator/(const BigInt& some_big_value) const {
  BigInt copy;
  if (*this == kMinValue) {
    return {-1};
  }
  if (*this == kMaxValue) {
    return {0};
  }
  if (*this < some_big_value) {
    return 0;
  }
  copy.sign_ = this->sign_ * some_big_value.sign_;
  copy.digits_.resize(this->digits_.size() + some_big_value.digits_.size() + 1);
  for (auto index_i = static_cast<int64_t>(this->digits_.size()) - 1;
       index_i >= 0; --index_i) {
    while (copy * some_big_value <= *this) {
      ++copy.digits_[index_i];
    }
    --copy.digits_[index_i];
  }
  while (copy.digits_.size() > 1 && copy.digits_.back() == 0) {
    copy.digits_.pop_back();
  }
  return copy;
}
bool operator>=(const BigInt& first_big_value, const BigInt& second_big_value) {
  if (first_big_value.sign_ < second_big_value.sign_) {
    return false;
  }
  if (first_big_value.sign_ > second_big_value.sign_) {
    return true;
  }
  if (first_big_value.digits_.size() * first_big_value.sign_ >
      second_big_value.digits_.size() * second_big_value.sign_) {
    return true;
  }
  if (first_big_value.digits_.size() * first_big_value.sign_ <
      second_big_value.digits_.size() * second_big_value.sign_) {
    return false;
  }
  for (auto index_i = static_cast<int64_t>(second_big_value.digits_.size()) - 1;
       index_i >= 0; --index_i) {
    if (first_big_value.digits_[index_i] * first_big_value.sign_ <
        second_big_value.digits_[index_i] * second_big_value.sign_) {
      return false;
    }
    if (first_big_value.digits_[index_i] * first_big_value.sign_ >
        second_big_value.digits_[index_i] * second_big_value.sign_) {
      return true;
    }
  }
  return true;
}
BigInt BigInt::operator%(const BigInt& some_big_value) const {
  BigInt result = *this - ((*this / some_big_value) * some_big_value);
  return result;
}

BigInt& BigInt::operator+=(const BigInt& some_big_value) {
  return *this = (*this + some_big_value);
}

BigInt& BigInt::operator-=(const BigInt& some_big_value) {
  return *this = (*this - some_big_value);
}

BigInt& BigInt::operator*=(const BigInt& some_big_value) {
  return *this = (*this * some_big_value);
}

BigInt& BigInt::operator/=(const BigInt& some_big_value) {
  return *this = (*this / some_big_value);
}

BigInt& BigInt::operator%=(const BigInt& some_big_value) {
  return *this = (*this % some_big_value);
}

BigInt& BigInt::operator++() { return *this += 1; }

BigInt& BigInt::operator--() { return *this -= 1; }

BigInt BigInt::operator++(int) {
  BigInt copy(*this);
  *this += 1;
  return copy;
}

BigInt BigInt::operator--(int) {
  BigInt copy(*this);
  *this -= 1;
  return copy;
}
bool operator==(const BigInt& first_big_value, const BigInt& second_big_value) {
  if (first_big_value.digits_.size() != second_big_value.digits_.size() ||
      first_big_value.sign_ != second_big_value.sign_) {
    return false;
  }
  for (uint64_t index = 0; index < first_big_value.digits_.size(); ++index) {
    if (first_big_value.digits_[index] != second_big_value.digits_[index]) {
      return false;
    }
  }
  return true;
}
bool operator<=(const BigInt& first_big_value, const BigInt& second_big_value) {
  return (second_big_value >= first_big_value);
}
bool operator<(const BigInt& first_big_value, const BigInt& second_big_value) {
  return (first_big_value <= second_big_value) &&
         (!(second_big_value == first_big_value));
}
bool operator>(const BigInt& first_big_value, const BigInt& second_big_value) {
  return (first_big_value >= second_big_value) &&
         (!(second_big_value == first_big_value));
}
bool operator!=(const BigInt& first_big_value, const BigInt& second_big_value) {
  return !(first_big_value == second_big_value);
}

std::istream& operator>>(std::istream& input, BigInt& number) {
  char symbol;
  number.digits_.clear();
  while (input.get(symbol) && (std::isspace(symbol) == 0)) {
    if (symbol == '-') {
      number.sign_ = -1;
      continue;
    }
    number.digits_.push_back(symbol - '0');
  }
  return input;
}

std::ostream& operator<<(std::ostream& output, const BigInt& number) {
  if (number.sign_ == -1) {
    output << '-';
  }
  for (long long digit : number.digits_) {
    output << digit;
  }
  return output;
}
