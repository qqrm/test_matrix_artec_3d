// Тестовое задание:
// Написать класс, представляющий матрицу m*n, где и m,n - небольшие числа, со
// следующими возможностями:
// [+] - поддержка перемещения, копирования
// [+] - возможность итерирования элементов в цикле вида for (auto v : m), где m
// - экземпляр класса матрицы
// [+] - конкатенация матриц через оператор |
// [+] - оператор индексации элементов
// [+] - сложение матриц

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

template <class T>
using v = std::vector<T>;

template <class T>
using vv = v<v<T>>;

template <class T>
using vit = typename v<T>::iterator;

template <class T>
class SimpleMatrix {
  class MatrixSize;

  v<T> _data;
  MatrixSize _sizes{0, 0};

  void add_zero_rows(size_t const count);
  void only_debug_sync_check() const;

 public:
  class AccessProxy;

  SimpleMatrix() = default;
  SimpleMatrix(SimpleMatrix const&) = default;
  SimpleMatrix(v<T> const& vec, size_t const n);
  SimpleMatrix(vv<T> const& vecs);
  SimpleMatrix(SimpleMatrix& matrix)
      : _data(matrix._data), _sizes(matrix._sizes) {}
  SimpleMatrix& operator=(SimpleMatrix const& r) {
    _data = r._data;
    _sizes = r._sizes;
    return *this;
  }
  SimpleMatrix(SimpleMatrix&& matrix) noexcept = default;
  SimpleMatrix& operator=(SimpleMatrix&&) noexcept = default;
  SimpleMatrix operator|(SimpleMatrix r);
  AccessProxy operator[](size_t const m);

  SimpleMatrix operator+(const SimpleMatrix<T>& r) const;

  std::optional<T> TryGet(size_t const m, size_t const n) const;

  bool operator==(const SimpleMatrix<T>& rhs) const {
    return _data == rhs._data && size() == rhs.size();
  }

  bool operator!=(const SimpleMatrix<T>& rhs) const {
    return _data != rhs._data || size() != rhs.size();
  }

  size_t m() const { return _sizes.m(); };
  size_t n() const { return _sizes.n(); };

  std::tuple<size_t, size_t> size() const { return {m(), n()}; };
  bool empty() const { return _sizes.empty(); };
  auto cbegin() const { return _data.cbegin(); };
  auto cend() const { return _data.cend(); };

  auto begin() { return _data.begin(); };
  auto end() { return _data.end(); };

#ifndef NDEBUG
  void print() const {
    for (size_t i = 0; i < m(); i++) {
      for (size_t j = 0; j < n(); j++) {
        std::cout << TryGet(i, j).value_or(0) << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
#endif
};

template <class T>
std::optional<T> SimpleMatrix<T>::TryGet(size_t const m, size_t const n) const {
  if (m < this->m() && n < this->n()) {
    return _data[m * this->n() + n];
  }

  return {};
}

template <class T>
SimpleMatrix<T> SimpleMatrix<T>::operator+(const SimpleMatrix<T>& r) const {
  size_t m{std::max(this->m(), r.m())};
  size_t n{std::max(this->n(), r.n())};

  SimpleMatrix temp(v<T>(m * n, 0), n);

  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      T sum{0};
      if (std::optional<T> el = this->TryGet(i, j); el.has_value()) {
        sum += el.value();
      }

      if (std::optional<T> el = r.TryGet(i, j); el.has_value()) {
        sum += el.value();
      }
      temp[i][j] = sum;
    }
  }
  temp.only_debug_sync_check();
  return temp;
}

template <class T>
class SimpleMatrix<T>::AccessProxy {
  vit<T> _i;

 public:
  AccessProxy(vit<T> i) : _i(i){};
  T& operator[](size_t const n) {
    std::advance(_i, n);
    return *_i;
  }
};

template <class T>
class SimpleMatrix<T>::MatrixSize {
  size_t _m{0};  // row count
  size_t _n{0};  // column count

 public:
  MatrixSize(size_t const m, size_t const n) : _m(m), _n(n) {}
  MatrixSize& operator=(MatrixSize const& r) {
    _m = r._m;
    _n = r._n;
    return *this;
  }
  MatrixSize(MatrixSize& size) : _m(size._m), _n(size._n) {}
  MatrixSize(MatrixSize&& size) noexcept : _m(size._m), _n(size._n) {
    size.clean();
  }
  MatrixSize& operator=(MatrixSize&& size) noexcept {
    _m = size._m;
    _n = size._n;
    size.clean();
    return *this;
  }

  size_t m() const { return _m; };
  size_t n() const { return _n; };
  void set(size_t const m, size_t const n) {
    _m = m;
    _n = n;
  }
  void clean() { set(0, 0); }
  bool empty() const { return _m == 0 && _n == 0; }
};

template <class T>
void SimpleMatrix<T>::only_debug_sync_check() const {
#ifndef NDEBUG
  assert(_data.size() == m() * n());
#endif
}

template <class T>
void SimpleMatrix<T>::add_zero_rows(size_t const count) {
  _data.insert(_data.end(), count * n(), 0);

  auto const [m, n] = size();
  _sizes.set(m + count, n);

  only_debug_sync_check();
}

template <class T>
SimpleMatrix<T> SimpleMatrix<T>::operator|(SimpleMatrix<T> r) {
  if (int const diff_m{static_cast<int>(m()) - static_cast<int>(r.m())};
      diff_m < 0) {
    add_zero_rows(abs(diff_m));
  } else {
    r.add_zero_rows(abs(diff_m));
  }

  std::vector<T> temp;
  auto const temp_size{_data.size() + r._data.size()};
  temp.reserve(temp_size);

  auto current_start{begin()};
  auto current_end{begin()};

  auto b_start{r.begin()};
  auto b_end{r.begin()};

  while (true) {
    if (current_end != end()) {
      std::advance(current_end, n());
      temp.insert(temp.end(), current_start, current_end);
      current_start = current_end;
    }

    if (b_end != r._data.end()) {
      std::advance(b_end, r.n());
      temp.insert(temp.end(), b_start, b_end);
      b_start = b_end;
    }

    if (temp.size() == temp_size) {
      break;
    }
  }

  SimpleMatrix<T> temp_matrix(std::move(temp), n() + r.n());
  temp_matrix.only_debug_sync_check();

  return temp_matrix;
}

template <class T>
SimpleMatrix<T>::SimpleMatrix(v<T> const& vec, size_t const n) {
  if (vec.empty() || n == vec.size()) {
    _data = vec;
    _sizes.set(1, n);
    only_debug_sync_check();
    return;
  }

  if (n > vec.size()) {
    v<T> temp = vec;
    temp.resize(n);
    _data = temp;
    _sizes.set(1, n);
    only_debug_sync_check();
    return;
  }

  size_t m = (vec.size() % n) ? (vec.size() / n + 1) : (vec.size() / n);
  size_t last = vec.size() % m;

  if (last == 0) {
    _data = vec;
    _sizes.set(m, n);
    only_debug_sync_check();
    return;
  }

  v<T> temp = vec;
  temp.insert(temp.end(), n - last, 0);
  _data = temp;
  _sizes.set(m, n);
  only_debug_sync_check();
}

template <class T>
SimpleMatrix<T>::SimpleMatrix(vv<T> const& vecs) {
  size_t max_size{0};

  for (auto const& cur : vecs) {
    max_size = std::max(max_size, cur.size());
  }

  _data.reserve(max_size * vecs.size());
  _sizes.set(vecs.size(), max_size);

  for (auto const& cur : vecs) {
    _data.insert(_data.end(), cur.begin(), cur.end());
    if (auto diff = max_size - cur.size(); diff > 0) {
      _data.insert(_data.end(), diff, 0);
    }
  }

  only_debug_sync_check();
}

template <class T>
typename SimpleMatrix<T>::AccessProxy SimpleMatrix<T>::operator[](
    size_t const m) {
  auto it = _data.begin();
  std::advance(it, m * n());
  return SimpleMatrix<T>::AccessProxy(it);
}
