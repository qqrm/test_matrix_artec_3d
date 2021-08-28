#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
#include <initializer_list>
#include <compare>
#include <exception>

namespace matrix
{
  template <class T, size_t ROW, size_t COL>
  class AccessProxy
  {
    std::vector<T>::iterator i_;

  public:
    AccessProxy() = default;
    void set(std::vector<T>::iterator i) { i_ = i; }
    constexpr T &operator[](size_t const n)
    {
      if (n >= COL)
      {
        throw std::out_of_range("n >= COL");
      }
      std::advance(i_, n);
      return *i_;
    }
  };
  template <class T, size_t ROW, size_t COL>
  class SimpleMatrix
  {
    std::vector<T> data_;

    AccessProxy<T, ROW, COL> proxy_;

  public:
    SimpleMatrix() : data_(ROW * COL){};
    explicit SimpleMatrix(std::initializer_list<T> init_list) : data_(init_list)
    {
      if (init_list.size() != ROW * COL)
      {
        throw std::invalid_argument("invalid elements count");
      }
    }

    SimpleMatrix(const SimpleMatrix &m) = default;
    SimpleMatrix &operator=(SimpleMatrix const &r) = default;
    SimpleMatrix(SimpleMatrix &&m) noexcept = default;
    SimpleMatrix &operator=(SimpleMatrix &&m) noexcept = default;

    // TODO: didn't  provide operator== on gcc 11.1
    auto operator<=>(const SimpleMatrix &rhs) const
    {
      return data_ <=> rhs.data_;
    }

    auto operator==(const SimpleMatrix &rhs) const
    {
      return data_ == rhs.data_;
    }

    constexpr T const &at(size_t const r, size_t const c) const
    {
      return data_.at(r * COL + c);
    }

    AccessProxy<T, ROW, COL> operator[](size_t const m)
    {
      if (m >= ROW)
      {
        throw std::out_of_range("m >= ROW");
      }

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
}