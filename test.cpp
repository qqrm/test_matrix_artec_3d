#include "include/acutest.h"
#include "matrix/matrix.hpp"
#include "matrix/matrix_ops.hpp"

#include <exception>
#include <sstream>

template class SimpleMatrix<int, 3, 5>;
using Matrix3x5 = SimpleMatrix<int, 3, 5>;

void init()
{
    Matrix3x5 empty;

    {
        Matrix3x5 v{
            0, 1, 2, 3, 4,
            5, 6, 7, 8, 9,
            8, 7, 6, 5, 4};

        Matrix3x5 a{v};
        assert(a == v);

        Matrix3x5 b = Matrix3x5{
            0, 1, 2, 3, 4,
            5, 6, 7, 8, 9,
            8, 7, 6, 5, 4};

        assert(v == b);
    }
}

void bad_init()
{
    TEST_EXCEPTION(Matrix3x5 bad_init({0, 1}), std::invalid_argument);
}

void assign_test()
{
    Matrix3x5 orig{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    auto copy = orig;
    assert(copy == orig);
}

void move_test1()
{
    Matrix3x5 orig{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    auto copy = orig;
    auto moved = std::move(orig);

    assert(moved == copy);
}

void move_test2()
{
    Matrix3x5 orig{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    auto moved = Matrix3x5{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    assert(moved == orig);
}

void foreach_test()
{
    Matrix3x5 m{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    std::stringstream ss;
    for (auto &v : m)
    {
        ss << v << " ";
    }

    auto str = ss.str();

    assert(str == std::string("0 1 2 3 4 5 6 7 8 9 8 7 6 5 4 "));
}

void foreach_mod_test()
{
    Matrix3x5 m{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    for (auto &v : m)
    {
        v = 1;
    }

    Matrix3x5 ed{
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1};

    assert(ed == m);
}

void foreach_mod_test_err1()
{
    Matrix3x5 m{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    TEST_EXCEPTION(m[33][1] = 4, std::out_of_range);

}

void foreach_mod_test_err2()
{
    Matrix3x5 m{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    TEST_EXCEPTION(m[1][33] = 4, std::out_of_range);
}

void proxy_test()
{
  Matrix3x5 m{
      0, 1, 2, 3, 4,
      5, 6, 7, 8, 9,
      8, 7, 6, 5, 4};

  auto el00 = m[0][0];
  assert(el00 == 0);

  auto el01 = m[0][1];
  assert(el01 == 1);

  auto el10 = m[1][0];
  assert(el10 == 5);
}

void concat_test()
{
  SimpleMatrix<int, 3, 5> a{
      0, 1, 2, 3, 4,
      5, 6, 7, 8, 9,
      8, 7, 6, 5, 4};

  SimpleMatrix<int, 4, 3> b{
      0, 1, 2,
      3, 4, 5,
      6, 7, 8,
      9, 8, 7};

  auto d = matrix_ops::concat(a, b);

  SimpleMatrix<int, 4, 8> res{
      0, 1, 2, 3, 4, 0, 1, 2,
      5, 6, 7, 8, 9, 3, 4, 5,
      8, 7, 6, 5, 4, 6, 7, 8,
      0, 0, 0, 0, 0, 9, 8, 7};

  assert(res == d);
}

void mul_test()
{
  SimpleMatrix<int, 3, 5> a{
      0, 1, 2, 3, 4,
      5, 6, 7, 8, 9,
      8, 7, 6, 5, 4};

  SimpleMatrix<int, 5, 2> b{
      0, 1,
      3, 4,
      6, 7,
      9, 8,
      6, 5};

  auto d = matrix_ops::mul(a, b);

  SimpleMatrix<int, 3, 2> res{
      66, 62,
      186, 187,
      126, 138};

  assert(res == d);
}

void resize_test()
{
  SimpleMatrix<int, 3, 2> m{
      66, 62,
      186, 187,
      126, 138};

  auto d = matrix_ops::resize<4, 4>(m);

  SimpleMatrix<int, 4, 4> res{
      66, 62, 0, 0,
      186, 187, 0, 0,
      126, 138, 0, 0,
      0, 0, 0, 0};

  assert(res == d);
}

void sum_test()
{
  {
    Matrix3x5 a{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    Matrix3x5 b{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    auto c = matrix_ops::sum(a, b);

    Matrix3x5 res{
        0, 2, 4, 6, 8,
        10, 12, 14, 16, 18,
        16, 14, 12, 10, 8};

    assert(res == c);
  }

  {
    SimpleMatrix<int, 3, 3> a{
        0, 1, 2,
        5, 6, 7,
        8, 7, 6};

    SimpleMatrix<int, 2, 5> b{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9};

    auto c = matrix_ops::sum(a, b);

    Matrix3x5 res{
        0, 2, 4, 3, 4,
        10, 12, 14, 8, 9,
        8, 7, 6, 0, 0};

    assert(res == c);
  }
}

void oper_plus_test()
{
  {
    Matrix3x5 a{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    a = a + a;

    Matrix3x5 res{
        0, 2, 4, 6, 8,
        10, 12, 14, 16, 18,
        16, 14, 12, 10, 8};

    assert(a == res);
  }

  {
    Matrix3x5 a{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    auto a_c = a;

    a = a + a + a;

    Matrix3x5 res = a_c * 3;

    assert(a == res);
  }

  {
    SimpleMatrix<int, 2, 5> a{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9};

    SimpleMatrix<int, 2, 5> b{
        7, 4, 2, 8, 4,
        5, 1, 3, 8, 9};

    auto c = a + b;

    SimpleMatrix<int, 2, 5> res{
        7, 5, 4, 11, 8,
        10, 7, 10, 16, 18};

    assert(c == res);
  }
}

void prints()
{
  SimpleMatrix<int, 3, 3> a{
      0, 1, 2,
      5, 6, 7,
      8, 7, 6};

  SimpleMatrix<int, 2, 5> b{
      0, 1, 2, 3, 4,
      5, 6, 7, 8, 9};

// TODO: override operator<<
//   matrix_ops::prints(a, b);
}

void concat_n_test()
{
  SimpleMatrix<int, 2, 1> a{
      0,
      5};

  SimpleMatrix<int, 3, 1> b{
      2,
      4,
      6};

  SimpleMatrix<int, 1, 2> c{
      7, 7};

  auto d = matrix_ops::concat(a, b, c);

  SimpleMatrix<int, 3, 4> res{
      0, 2, 7, 7,
      5, 4, 0, 0,
      0, 6, 0, 0};

  assert(d == res);
}

void mul_n_test()
{
  SimpleMatrix<int, 2, 4> a{
      0, 3, 1, 3,
      5, 4, 0, 7};

  SimpleMatrix<int, 4, 3> b{
      2, 4, 6,
      4, 3, 6,
      6, 3, 1,
      2, 8, 9};

  SimpleMatrix<int, 3, 2> c{
      7, 1,
      5, 3,
      4, 7};

  auto d = matrix_ops::mul(a, b, c);

  SimpleMatrix<int, 2, 2> res{
      532, 454,
      1188, 1123};

  assert(d == res);
}

void sum_n_test()
{
  SimpleMatrix<int, 2, 3> a{
      0, 3, 1,
      5, 4, 0};

  SimpleMatrix<int, 1, 3> b{
      2, 4, 6};

  SimpleMatrix<int, 3, 1> c{
      7,
      5,
      4};

  auto d = matrix_ops::sum(a, b, c);

  SimpleMatrix<int, 3, 3> res{
      9, 7, 7,
      10, 4, 0,
      4, 0, 0};

  assert(d == res);
}

void compare_test()
{
  SimpleMatrix<int, 1, 3> a{
      2, 4, 6};

  SimpleMatrix<int, 1, 3> b{
      2, 4, 6};

  auto comp1 = a == b;
  assert(comp1 == true);

  SimpleMatrix<int, 1, 3> c{
      2, 4, 7};

  auto comp2 = a < c;
  assert(comp2 == true);

  SimpleMatrix<int, 1, 3> d{
      2, 3, 7};

  auto comp3 = a > d;
  assert(comp3 == true);
}


TEST_LIST = {

    {"init", init},
    {"bad init", bad_init},
    {"assign test", assign_test},
    {"move_test", move_test1},
    {"move_test", move_test2},
    {"foreach test", foreach_test},
    {"foreach mod test", foreach_mod_test},
    {"foreach mod test err row", foreach_mod_test_err1},
    {"foreach mod test err col", foreach_mod_test_err2},
    {"proxy acsess test", proxy_test},
    {"concat test", concat_test},
    {"mul test", mul_test},
    {"resize test", resize_test},
    {"sum test", sum_test},
    {"op plus test", oper_plus_test},
    // {"prints", prints},
    {"concat n  test", concat_n_test},
    {"mul n test", mul_n_test},
    {"sum_n_test", sum_n_test},
    {"compare test", compare_test},

    {NULL, NULL}};