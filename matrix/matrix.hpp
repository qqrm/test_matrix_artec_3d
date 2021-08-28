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
#include <initializer_list>

template <class T, size_t COL>
class AccessProxy
{
  std::vector<T>::iterator i_;

public:
  AccessProxy() = default;
  void set(std::vector<T>::iterator i) { i_ = i; }
  constexpr T &operator[](size_t const n)
  {
    assert(n < COL);
    std::advance(i_, n);
    return *i_;
  }
};
template <class T, size_t ROW, size_t COL>
class SimpleMatrix
{
  std::vector<T> data_;

  AccessProxy<T, COL> proxy_;
public:

  SimpleMatrix() : data_(ROW * COL){};
  explicit SimpleMatrix(std::initializer_list<T> init_list) : data_(init_list)
  {
    assert(init_list.size() == ROW * COL);
  }

  SimpleMatrix(const SimpleMatrix &m)
  {
    data_ = m.data_;
  }

  SimpleMatrix &operator=(SimpleMatrix const &r) = default;
  SimpleMatrix(SimpleMatrix &&m) noexcept = default;
  SimpleMatrix &operator=(SimpleMatrix &&m) noexcept = default;

  constexpr T const &at(size_t const r, size_t const c) const
  {
    return data_.at(r * COL + c);
  }

  bool operator==(const SimpleMatrix &rhs) const
  {
    return data_ == rhs.data_;
  }

  bool operator!=(const SimpleMatrix &rhs) const
  {
    return data_ != rhs.data_;
  }

  AccessProxy<T, COL> operator[](size_t const m)
  {
    auto it = data_.begin();
    std::advance(it, m * COL);
    proxy_.set(it);
    return proxy_;
  }

  friend SimpleMatrix operator+(SimpleMatrix lhs, const SimpleMatrix &rhs)
  {
    for (size_t i{0}; i < ROW; i++)
    {
      for (size_t j{0}; j < COL; j++)
      {
        lhs.data_[i * COL + j] = lhs.data_[i * COL + j] + rhs.data_[i * COL + j];
      }
    }
    return lhs;
  }

  friend SimpleMatrix operator*(SimpleMatrix lhs, const T n)
  {
    std::transform(lhs.data_.begin(), lhs.data_.end(), lhs.data_.begin(), [n](auto el) -> T
                   { return el * n; });
    return lhs;
  }

  auto cbegin() const { return data_.cbegin(); }
  auto cend() const { return data_.cend(); }

  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }

  void print() const
  {
    for (size_t i{0}; i < ROW; i++)
    {
      for (size_t j{0}; j < COL; j++)
      {
        std::cout << data_[i * COL + j] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
};

namespace matrix_ops
{
  template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
  auto concat(SimpleMatrix<T, ROW1, COL1> a, SimpleMatrix<T, ROW2, COL2> b)
  {
    size_t const ROW3 = std::max(ROW1, ROW2);
    size_t const COL3 = COL1 + COL2;

    SimpleMatrix<T, ROW3, COL3> result;

    for (size_t i{0}; i < ROW1; i++)
    {
      for (size_t j{0}; j < COL1; j++)
      {
        result[i][j] = a[i][j];
      }
    }

    for (size_t i{0}; i < ROW2; i++)
    {
      for (size_t j{0}; j < COL2; j++)
      {
        result[i][COL1 + j] = b[i][j];
      }
    }

    return result;
  };

  template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
  auto mul(SimpleMatrix<T, ROW1, COL1> a, SimpleMatrix<T, ROW2, COL2> b)
  {
    static_assert(ROW2 == COL1);

    size_t const ROW3 = ROW1;
    size_t const COL3 = COL2;

    SimpleMatrix<T, ROW3, COL3> result;

    for (size_t i{0}; i < ROW1; ++i)
    {
      for (size_t j{0}; j < COL2; ++j)
      {
        for (size_t k{0}; k < COL1; ++k)
        {
          result[i][j] += a[i][k] * b[k][j];
        }
      }
    }

    return result;
  };

  template <size_t NEW_ROW, size_t NEW_COL, class T, size_t ROW, size_t COL>
  auto resize(SimpleMatrix<T, ROW, COL> m)
  {
    SimpleMatrix<T, NEW_ROW, NEW_COL> result;

    for (size_t i{0}; i < ROW; i++)
    {
      for (size_t j{0}; j < COL; j++)
      {
        result[i][j] = m[i][j];
      }
    }

    return result;
  }

  template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
  auto sum(SimpleMatrix<T, ROW1, COL1> a_, SimpleMatrix<T, ROW2, COL2> b_)
  {
    size_t const ROW3 = std::max(ROW1, ROW2);
    size_t const COL3 = std::max(COL1, COL2);

    auto a = resize<ROW3, COL3>(a_);
    auto b = resize<ROW3, COL3>(b_);

    SimpleMatrix<T, ROW3, COL3> result;

    for (size_t i{0}; i < ROW3; i++)
    {
      for (size_t j{0}; j < COL3; j++)
      {
        result[i][j] = a[i][j] + b[i][j];
      }
    }

    return result;
  };
}