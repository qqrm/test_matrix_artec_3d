// Тестовое задание:
// Написать класс, представляющий матрицу m*n, где и m,n - небольшие числа, со
// следующими возможностями:
// [+] - поддержка перемещения, копирования
// [+] - возможность итерирования элементов в цикле вида for (auto v : m), где m
// - экземпляр класса матрицы
// [-] - конкатенация матриц через оператор |
// [-] - оператор индексации элементов
// [-] - сложение матриц

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

template <class T>
class SimpleMatrix {
  using v = std::vector<T>;
  using vv = std::vector<std::vector<T>>;

  struct MatrixSize {
    size_t _m{0};  // row count
    size_t _n{0};  // column count

    MatrixSize(size_t const m, size_t const n) : _m(m), _n(n) {}

    void set(size_t const m, size_t const n) {
      _m = m;
      _n = n;
    }
    void clean() { set(0, 0); }
    bool empty() const { return _m == 0 && _n == 0; }
  };

  // bool operator==(const MatrixSize& b) const {
  //  return this._m == b._m && this._n = b._n;
  //}

  MatrixSize _sizes{0, 0};
  std::vector<T> _data{0, 0};

 public:
  SimpleMatrix() = default;
  SimpleMatrix(const v& vec, size_t row_len);
  SimpleMatrix(vv vecs);
  SimpleMatrix(const SimpleMatrix&) = default;
  SimpleMatrix(SimpleMatrix&& matrix) noexcept;

  std::tuple<size_t, size_t> size() const { return {_sizes._m, _sizes._n}; };
  bool empty() const { return _sizes.empty(); };
  auto begin() const { return _data.begin(); };
  auto end() const { return _data.end(); };
};

template <class T>
inline SimpleMatrix<T>::SimpleMatrix(const v& vec, size_t row_len)
    : _data(vec), _sizes(_data, row_len) {}

template <class T>
inline SimpleMatrix<T>::SimpleMatrix(vv vecs) {
  size_t max_size{0};

  for (auto const& cur : vecs) {
    max_size = std::max(max_size, cur.size());
  }
  _data.reserve(max_size * vecs.size());

  _sizes.set(vecs.size(), max_size);

  for (auto const& cur : vecs) {
    _data.insert(_data.end(), cur.begin(), cur.end());
    if (auto diff = max_size - cur.size(); diff > 0) {
      _data.insert(_data.end(), 0, diff);
    }
  }
}

template <class T>
inline SimpleMatrix<T>::SimpleMatrix(SimpleMatrix&& matrix) noexcept
    : _data(std::move(matrix._data)) {}
