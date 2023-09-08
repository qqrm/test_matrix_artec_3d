#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <ranges>

namespace matrix
{

  // AccessProxy: An iterator-like object for accessing elements within a row of the matrix.
  template <typename T, size_t COL>
  class AccessProxy
  {
    typename std::vector<T>::iterator i_; // Iterator pointing to a position within the matrix.

  public:
    AccessProxy() = default;

    // Set the iterator to a specific position within the matrix.
    void set(typename std::vector<T>::iterator i)
    {
      i_ = i;
    }

    // Access elements in the row represented by this proxy.
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

  // SimpleMatrix: A simple matrix data structure.
  template <typename T, size_t ROW, size_t COL>
  class SimpleMatrix
  {
    std::vector<T> data_;
    AccessProxy<T, COL> proxy_;

  public:
    // Constructor: Initialize the matrix with default-initialized elements.
    SimpleMatrix() : data_(ROW * COL) {}

    // Constructor: Initialize the matrix with elements from an initializer list.
    explicit SimpleMatrix(std::initializer_list<T> init_list) : data_(init_list)
    {
      if (init_list.size() != ROW * COL)
      {
        throw std::invalid_argument("Invalid initializer list size");
      }
    }

    // Copy constructor.
    SimpleMatrix(const SimpleMatrix &m) = default;

    // Copy assignment operator.
    SimpleMatrix &operator=(const SimpleMatrix &r) = default;

    // Move constructor.
    SimpleMatrix(SimpleMatrix &&m) noexcept = default;

    // Move assignment operator.
    SimpleMatrix &operator=(SimpleMatrix &&m) noexcept = default;

    // Access an element at a specific row and column.
    constexpr T const &at(size_t const r, size_t const c) const
    {
      return data_.at(r * COL + c);
    }

    // Access a row using the [] operator and return an AccessProxy.
    constexpr AccessProxy<T, COL> &operator[](size_t const r)
    {
      if (r >= ROW)
      {
        throw std::out_of_range("r >= ROW");
      }

      auto it = data_.begin();
      std::advance(it, r * COL);
      proxy_.set(it);
      return proxy_;
    }

    // Addition operator for matrix addition.
    friend SimpleMatrix operator+(SimpleMatrix lhs, const SimpleMatrix &rhs)
    {
      for (size_t i = 0; i < ROW * COL; i++)
      {
        lhs.data_[i] += rhs.data_[i];
      }
      return lhs;
    }

    // Scalar multiplication operator.
    friend SimpleMatrix operator*(SimpleMatrix lhs, const T n)
    {
      std::ranges::transform(lhs.data_.begin(), lhs.data_.end(), lhs.data_.begin(),
                             [&n](auto &el)
                             { return el * n; });
      return lhs;
    }

    // Constant iterator for the beginning of the matrix.
    auto cbegin() const
    {
      return data_.cbegin();
    }

    // Constant iterator for the end of the matrix.
    auto cend() const
    {
      return data_.cend();
    }

    // Iterator for the beginning of the matrix.
    auto begin()
    {
      return data_.begin();
    }

    // Iterator for the end of the matrix.
    auto end()
    {
      return data_.end();
    }

    // Output operator to display the matrix.
    friend std::ostream &operator<<(std::ostream &os, const SimpleMatrix &m)
    {
      for (size_t i = 0; i < ROW; i++)
      {
        for (size_t j = 0; j < COL; j++)
        {
          os << std::setw(3) << m.data_[i * COL + j] << " ";
        }
        os << "\n";
      }
      return os;
    }

    // Input operator to read values into the matrix.
    friend std::istream &operator>>(std::istream &is, SimpleMatrix &m)
    {
      for (size_t i = 0; i < ROW * COL; i++)
      {
        is >> m.data_[i];
      }
      return is;
    }

    // Utility method to print the matrix.
    void print()
    {
      std::cout << "\nMatrix " << ROW << "x" << COL << ":\n"
                << *this;
    }
  }; // SimpleMatrix

} // namespace matrix
