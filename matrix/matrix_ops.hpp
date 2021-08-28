#pragma once

#include "matrix.hpp"

namespace matrix_ops
{

    template <typename... SimpleMatrix>
    void prints(SimpleMatrix &...matrix)
    {
        (..., matrix.print());
        std::cout << '\n';
    }

    template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
    auto concat(const SimpleMatrix<T, ROW1, COL1> &a, const SimpleMatrix<T, ROW2, COL2> &b)
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

    // template <typename... SimpleMatrix>
    // void concat_mul(SimpleMatrix &...matrix)
    // {
    //     auto result = concat(matrix, (...));
    // }

    // template <typename... SimpleMatrix>
    // auto concat_mul(SimpleMatrix &&...matrix)
    // {
    //     //    return (args + ... + 1 * 2); // Error: operator with precedence below cast
    //     // bool all(Args... args) { return (... && args); }
    //     return concat(matrix, ...);
    //     // return (args + ... + (1 * 2)); // OK
    // }

    // template <typename SimpleMatrix, typename... Args>
    // SimpleMatrix concat_mul(SimpleMatrix matrix, Args... args)
    // {
    //     auto concat_one = [matrix](SimpleMatrix &b)
    //     {
    //         return concat(matrix, b);
    //     };

    //     return concat_one(...);
    // }


    // template <typename T1, typename T2>
    // auto adder(const T1 &t1, const T2 &t2)
    // {
    //     return t1 + t2;
    // }

    // template <typename T1, typename... T2>
    // auto adder(const T1 &t1, const T2 &...t2)
    // {
    //     return t1 + adder(t2...);
    // }

    template <typename SimpleMatrix1, typename... SimpleMatrix2>
    auto concat_mul(const SimpleMatrix1 &m1, const SimpleMatrix2 & ...m2)
    {
        // return t1 + adder(t2...);
        return concat(m1, concat(m2...));
    }

    template <class T, size_t ROW1, size_t COL1, size_t ROW2, size_t COL2>
    auto mul(const SimpleMatrix<T, ROW1, COL1> &a, const SimpleMatrix<T, ROW2, COL2> &b)
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
    };

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
    auto sum(const SimpleMatrix<T, ROW1, COL1> &a_, const SimpleMatrix<T, ROW2, COL2> &b_)
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