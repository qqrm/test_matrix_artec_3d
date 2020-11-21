#include "matrix_test.h"

#include <cassert>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

#include "matrix/matrix.hpp"

template class SimpleMatrix<int>;

void init_matrix_test() {
  SimpleMatrix<int> empty;
  assert(empty.empty());

  {
    vv<int> const v{{0, 1, 2, 3}, {4, 5, 6, 7}};
    SimpleMatrix<int> rect(v);
    [[maybe_unused]] auto const [m, n] = rect.size();
    assert(m == 2 && n == 4);
  }

  {
    vv<int> const v{{0, 1}, {2, 3}, {4, 5}, {6, 7}};
    SimpleMatrix<int> rect(v);
    [[maybe_unused]] auto const [m, n] = rect.size();
    assert(m == 4 && n == 2);
  }

  {
    vv<int> v{{0, 1}, {2, 3}, {4, 5}, {6, 7}};
    SimpleMatrix<int> a(v);
    SimpleMatrix<int> b;
    b = std::move(a);
    assert(a.empty());
    [[maybe_unused]] auto const [m, n] = b.size();
    assert(m == 4 && n == 2);
  }

   {
     SimpleMatrix<int> bad_init1({0, 1}, 7);
     SimpleMatrix<int> bad_init2({0, 1}, 2);
     SimpleMatrix<int> bad_init3({0, 1}, 3);
     SimpleMatrix<int> bad_init4({0, 1, 2, 3, 4}, 4);
  }
}

void init_non_normal_test() {
  {
    vv<int> const non_normalized_data{{1}, {1, 2}};
    SimpleMatrix<int> filled_normalize(non_normalized_data);
    assert(filled_normalize == SimpleMatrix<int>({{1, 0}, {1, 2}}));
  }

  {
    vv<int> const non_normalized_data{{1}, {}, {1, 2}};
    SimpleMatrix<int> filled_normalize(non_normalized_data);
    assert(filled_normalize == SimpleMatrix<int>({{1, 0}, {0, 0}, {1, 2}}));
  }
}

void assign_test() {
  SimpleMatrix<int> filled({{0, 1, 2, 3}, {4, 5, 6, 7}});
  auto test_copy = filled;
  assert(test_copy.size() == filled.size() && !filled.empty());
}

void move_test() {
  SimpleMatrix<int> filled({{0, 1, 2, 3}, {4, 5, 6, 7}});
  auto m_test_move = std::move(filled);
  assert(!m_test_move.empty() && filled.empty());
}

void foreach_test() {
  vv<int> const matrix_data{{0, 1, 2, 3}, {4, 5, 6, 7}};

  SimpleMatrix<int> m(matrix_data);
  std::stringstream ss;
  for (auto& v : m) {
    ss << v << " ";
  }

  assert(ss.str() == [&]() {
    size_t matrix_data_size{0};
    std::string test_str;

    for (auto const& cur : matrix_data) matrix_data_size += cur.size();

    for (auto const& cur : matrix_data)
      for (auto const& el : cur) test_str += std::to_string(el) + " ";

    return test_str;
  }());
}

void foreach_mod_test() {
  SimpleMatrix<int> m({{0, 1, 2, 3}, {4, 5, 6, 7}});
  for (auto& v : m) {
    v = 1;
  }
  assert(SimpleMatrix<int>({{1, 1, 1, 1}, {1, 1, 1, 1}}) == m);
}

void concat_test() {
  SimpleMatrix<int> a({{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}});
  SimpleMatrix<int> b({{-1, -2}, {-3, -4}});
  auto d = a | b;
  assert(SimpleMatrix<int>({{0, 1, 2, 3, -1, -2},
                            {4, 5, 6, 7, -3, -4},
                            {8, 9, 10, 11, 0, 0}}) == d);
}

void access_el_test() {
  assert(SimpleMatrix<int>({{1, 2}, {3, 4}})[1][1] == 4);
}

void access_el_mod_test() {
  auto m{SimpleMatrix<int>({{1, 2}, {3, 4}})};
  m[1][1] = 7;

  assert(SimpleMatrix<int>({{1, 2}, {3, 7}}) == m);
}

void sum_test() {
  auto a{SimpleMatrix<int>({{1, 2}, {3, 4}, {5, 6}})};
  auto b{SimpleMatrix<int>({{2, 2, 2, 2}, {3, 3, 3, 3}})};

  auto c = a + b;

  assert(SimpleMatrix<int>({{3, 4, 2, 2}, {6, 7, 3, 3}, {5, 6, 0, 0}}) == c);
}

int main() {
  init_matrix_test();
  init_non_normal_test();

  assign_test();
  move_test();

  foreach_test();
  foreach_mod_test();

  concat_test();

  access_el_test();
  access_el_mod_test();

  sum_test();

  return 0;
}
