#include <assert.h>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

#include "matrix/matrix.hpp"

template class SimpleMatrix<int, 3, 5>;

using Matrix3x5 = SimpleMatrix<int, 3, 5>;

void init_matrix_test()
{
  Matrix3x5 empty;

  {
    Matrix3x5 v{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    v.print();

    Matrix3x5 a{v};
    assert(a == v);

    Matrix3x5 b = Matrix3x5{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};

    assert(v == b);
  }

  {
    // Matrix bad_init({0, 1}); // error
  }
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

void move_test()
{
  Matrix3x5 orig{
      0, 1, 2, 3, 4,
      5, 6, 7, 8, 9,
      8, 7, 6, 5, 4};

  auto copy = orig;
  auto moved = std::move(orig);

  assert(moved == copy);
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

int main()
{
  init_matrix_test();

  assign_test();
  move_test();

  foreach_test();
  foreach_mod_test();

  proxy_test();

  concat_test();

  mul_test();

  resize_test();

  sum_test();

  oper_plus_test();

  return 0;
}
