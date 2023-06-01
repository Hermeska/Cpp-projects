//
// Created by Daniel on 06.12.2022.
//

#include <algorithm>
#include <iostream>
#include <vector>

template <uint64_t N, uint64_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix() : matrix_(N, std::vector<T>(M, T())){};
  T& operator()(uint64_t rows, uint64_t collumns);
  T operator()(uint64_t rows, uint64_t collumns) const;
  explicit Matrix(std::vector<std::vector<T>> some_array);
  Matrix(const Matrix<N, M, T>& some_matrix);
  Matrix<N, M, T>& operator=(const Matrix<N, M, T>& some_matrix) = default;
  Matrix<M, N, T> Transposed();
  explicit Matrix(T elem);
  Matrix<N, M, T> operator-(const Matrix<N, M, T>& some_matrix);
  Matrix<N, M, T> operator+(const Matrix<N, M, T>& some_matrix);
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& some_matrix);

  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& some_matrix);

  void Swap(Matrix<N, M, T>& some_matrix);

  void Print();

  Matrix<N, M, T> operator*(T value);

  Matrix<N, M, T>& operator*=(T value);
  template <uint64_t U, uint64_t Z, typename T1>
  bool operator==(const Matrix<U, Z, T1>& some_matrix);
  template <uint64_t U>
  Matrix<N, U, T> operator*(const Matrix<M, U, T>& some_matrix);

 private:
  std::vector<std::vector<T>> matrix_;
};
template <uint64_t N, typename T>
class Matrix<N, N, T> {
 public:
  Matrix() : matrix_(N, std::vector<T>(N, T())){};
  T& operator()(uint64_t rows, uint64_t collumns);
  T operator()(uint64_t rows, uint64_t collumns) const;
  explicit Matrix(std::vector<std::vector<T>> some_vector);
  Matrix(const Matrix<N, N, T>& some_matrix);
  Matrix<N, N, T>& operator=(const Matrix<N, N, T>& some_matrix) = default;

  explicit Matrix(T elem);

  Matrix<N, N, T>& operator+=(const Matrix<N, N, T>& some_matrix);

  Matrix<N, N, T>& operator-=(const Matrix<N, N, T>& some_matrix);

  void Swap(Matrix<N, N, T>& some_matrix);

  void Print();

  Matrix<N, N, T> operator*(T value);

  Matrix<N, N, T>& operator*=(T value);
  Matrix<N, N, T> operator*(const Matrix<N, N, T>& some_matrix);
  Matrix<N, N, T> Transposed();
  T Trace();
  template <uint64_t U, typename T1>
  bool operator==(const Matrix<U, U, T1>& some_matrix);
  Matrix<N, N, T> operator-(const Matrix<N, N, T>& some_matrix);
  Matrix<N, N, T> operator+(const Matrix<N, N, T>& some_matrix);

 private:
  std::vector<std::vector<T>> matrix_;
};
template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>::Matrix(std::vector<std::vector<T>> some_array)
    : matrix_(N, std::vector<T>(M)) {
  if (!some_array.empty()) {
    matrix_ = some_array;
  }
}
template <uint64_t N, typename T>
Matrix<N, N, T>::Matrix(std::vector<std::vector<T>> some_vector)
    : matrix_(N, std::vector<T>(N)) {
  if (!some_vector.empty()) {
    matrix_ = some_vector;
  }
}

template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>::Matrix(T elem) : matrix_(N, std::vector<T>(M)) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      matrix_[i][j] = elem;
    }
  }
}
template <uint64_t N, typename T>
Matrix<N, N, T>::Matrix(T elem) : matrix_(N, std::vector<T>(N)) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      matrix_[i][j] = elem;
    }
  }
}

template <uint64_t N, uint64_t M, typename T>
void Matrix<N, M, T>::Print() {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      std::cout << matrix_[i][j] << " ";
    }
    std::cout << "\n";
  }
}
template <uint64_t N, typename T>
void Matrix<N, N, T>::Print() {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      std::cout << matrix_[i][j] << " ";
    }
    std::cout << "\n";
  }
}

template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(T value) {
  Matrix<N, M, T> copy = *this;
  copy *= value;
  return copy;
}
template <uint64_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator*(T value) {
  Matrix<N, N, T> copy = *this;
  copy *= value;
  return copy;
}

template <uint64_t N, uint64_t M, typename T>
void Matrix<N, M, T>::Swap(Matrix<N, M, T>& some_matrix) {
  std::swap(matrix_, some_matrix.matrix_);
}
template <uint64_t N, typename T>
void Matrix<N, N, T>::Swap(Matrix<N, N, T>& some_matrix) {
  std::swap(matrix_, some_matrix.matrix_);
}

template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(T value) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      this->operator()(i, j) *= value;
    }
  }
  return *this;
}
template <uint64_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator*=(T value) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      this->operator()(i, j) *= value;
    }
  }
  return *this;
}

template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(
    const Matrix<N, M, T>& some_matrix) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      this->operator()(i, j) += some_matrix(i, j);
    }
  }
  return *this;
}
template <uint64_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator+=(
    const Matrix<N, N, T>& some_matrix) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      this->opertor()(i, j) += some_matrix(i, j);
    }
  }
  return *this;
}

template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(
    const Matrix<N, M, T>& some_matrix) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      this->operator()(i, j) -= some_matrix(i, j);
    }
  }
  return *this;
}
template <uint64_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator-=(
    const Matrix<N, N, T>& some_matrix) {
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      this->operator()(i, j) -= some_matrix(i, j);
    }
  }
  return *this;
}

template <uint64_t N, uint64_t M, typename T>
template <uint64_t U>
Matrix<N, U, T> Matrix<N, M, T>::operator*(const Matrix<M, U, T>& some_matrix) {
  Matrix<N, U, T> new_matrix;
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < U; ++j) {
      for (uint64_t temp = 0; temp < M; ++temp) {
        new_matrix(i, j) += this->matrix_[i][temp] * some_matrix(temp, j);
      }
    }
  }
  return new_matrix;
}

template <uint64_t N, uint64_t M, typename T>
T& Matrix<N, M, T>::operator()(uint64_t rows, uint64_t collumns) {
  return matrix_[rows][collumns];
}
template <uint64_t N, typename T>
T& Matrix<N, N, T>::operator()(uint64_t rows, uint64_t collumns) {
  return matrix_[rows][collumns];
}

template <uint64_t N, uint64_t M, typename T>
T Matrix<N, M, T>::operator()(uint64_t rows, uint64_t collumns) const {
  return matrix_[rows][collumns];
}
template <uint64_t N, uint64_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> trans_matrix;
  if (!matrix_.empty()) {
    for (uint64_t i = 0; i < N; ++i) {
      for (uint64_t j = 0; j < M; ++j) {
        trans_matrix(j, i) = this->operator()(i, j);
      }
    }
  }
  return trans_matrix;
}
template <uint64_t N, uint64_t M, typename T>
template <uint64_t U, uint64_t Z, typename T1>
bool Matrix<N, M, T>::operator==(const Matrix<U, Z, T1>& some_matrix) {
  if (U != N || M != Z || !std::is_same<T1, T>::value) {
    return false;
  }
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < M; ++j) {
      if (this->operator()(i, j) != some_matrix(i, j)) {
        return false;
      }
    }
  }
  return true;
}
template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& some_matrix) {
  Matrix<N, M, T> copy = *this;
  copy -= some_matrix;
  return copy;
}
template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix<N, M, T>& some_matrix) {
  Matrix<N, M, T> copy = *this;
  copy += some_matrix;
  return copy;
}
template <uint64_t N, uint64_t M, typename T>
Matrix<N, M, T>::Matrix(const Matrix<N, M, T>& some_matrix)
    : matrix_(some_matrix.matrix_) {}

template <uint64_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::Transposed() {
  Matrix<N, N, T> trans_matrix;
  if (matrix_.empty() == 0) {
    for (uint64_t i = 0; i < N; ++i) {
      for (uint64_t j = 0; j < N; ++j) {
        trans_matrix(i, j) = this->operator()(j, i);
      }
    }
  }
  return trans_matrix;
}
template <uint64_t N, typename T>
T Matrix<N, N, T>::operator()(uint64_t rows, uint64_t collumns) const {
  return matrix_[rows][collumns];
}
template <uint64_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator*(const Matrix<N, N, T>& some_matrix) {
  Matrix<N, N, T> new_matrix;
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      for (uint64_t temp = 0; temp < N; ++temp) {
        new_matrix(i, j) += this->matrix_[i][temp] * some_matrix(temp, j);
      }
    }
  }
  return new_matrix;
}

template <uint64_t N, typename T>
T Matrix<N, N, T>::Trace() {
  T result = T();
  for (uint64_t i = 0; i < N; ++i) {
    result += this->operator()(i, i);
  }
  return result;
}
template <uint64_t N, typename T>
Matrix<N, N, T>::Matrix(const Matrix<N, N, T>& some_matrix)
    : matrix_(some_matrix.matrix_) {}
template <uint64_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator-(const Matrix<N, N, T>& some_matrix) {
  Matrix<N, N, T> copy = *this;
  copy -= some_matrix;
  return copy;
}
template <uint64_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator+(const Matrix<N, N, T>& some_matrix) {
  Matrix<N, N, T> copy = *this;
  copy -= some_matrix;
  return copy;
}
template <uint64_t N, typename T>
template <uint64_t U, typename T1>
bool Matrix<N, N, T>::operator==(const Matrix<U, U, T1>& some_matrix) {
  if (U != N || !std::is_same<T1, T>::value) {
    return false;
  }
  for (uint64_t i = 0; i < N; ++i) {
    for (uint64_t j = 0; j < N; ++j) {
      if (this->opertor()(i, j) != some_matrix(i, j)) {
        return false;
      }
    }
  }
  return true;
}
