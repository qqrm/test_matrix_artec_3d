// matrix_test.cpp : Defines the entry point for the application.
//

#include "matrix_test.h"

#include <cassert>
#include <sstream>

using namespace std;

int main() {
  cout << "Hello CMake." << endl;

  std::vector<std::vector<int>> const matrix = {
      {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}};

  std::string const test_elements = "1 2 3 4 5 6 7 8 9 10 11";

  SimpleMatrix<int> m_empty;
  SimpleMatrix<int> m_filled(matrix);
  SimpleMatrix<int> m_filled_bad({{1}, {1, 2}});
  assert(m_filled_bad.empty());

  auto m_test_copy = m_filled;
  assert(m_test_copy.size() == m_filled.size());

  auto m_test_move = std::move(m_filled);
  assert(!m_test_move.empty() && m_filled.empty());

  {
    std::stringstream ss;
    for (auto& v : m_test_move) {
      ss << v;
    }
    assert(ss.str() == test_elements);
  }

  {
    std::stringstream ss;
    for (auto const& v : m_test_move) {
      std::cout << v << '\n';
    }
    assert(ss.str() == test_elements);
  }



  return 0;
};
