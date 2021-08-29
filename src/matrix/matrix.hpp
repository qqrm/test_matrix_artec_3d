#pragma once

#include "matrix_base.hpp"

namespace matrix
{
    template <typename... SimpleMatrix>
    void prints(SimpleMatrix &...matrix)
    {
        (..., matrix.print());
        std::cout << '\n';
    }

    template <size_t NEW_ROW, size_t NEW_COL, class T, size_t ROW, size_t COL>
    auto resize(const SimpleMatrix<T, ROW, COL> &m)
    {
        SimpleMatrix<T, NEW_ROW, NEW_COL> result;

        for (size_t i{0}; i < ROW; i++)
        {
            for (size_t j{0}; j < COL; j++)
            {
                result[i][j] = m.at(i, j);
            }
        }

        return result;
    }

    template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
    auto operator*(const SimpleMatrix<T, ROW1, COL1> &a, const SimpleMatrix<T, ROW2, COL2> &b)
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
                    result[i][j] += a.at(i, k) * b.at(k, j);
                }
            }
        }

        return result;
    }

    template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
    auto operator+(const SimpleMatrix<T, ROW1, COL1> &a_, const SimpleMatrix<T, ROW2, COL2> &b_)
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

    template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
    auto operator|(const SimpleMatrix<T, ROW1, COL1> &a, const SimpleMatrix<T, ROW2, COL2> &b)
    {
        size_t const ROW3 = std::max(ROW1, ROW2);
        size_t const COL3 = COL1 + COL2;

        SimpleMatrix<T, ROW3, COL3> result;

        for (size_t i{0}; i < ROW1; i++)
        {
            for (size_t j{0}; j < COL1; j++)
            {
                result[i][j] = a.at(i, j);
            }
        }

        for (size_t i{0}; i < ROW2; i++)
        {
            for (size_t j{0}; j < COL2; j++)
            {
                result[i][COL1 + j] = b.at(i, j);
            }
        }

        return result;
    };

} // matrix