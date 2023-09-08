#include "include/acutest.h"
#include "matrix/matrix.hpp"

using namespace matrix;

using Matrix3x5 = SimpleMatrix<int, 3, 5>;

// Helper function to create a sample matrix for testing
Matrix3x5 createSampleMatrix()
{
    return Matrix3x5{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        8, 7, 6, 5, 4};
}

// Test matrix initialization
void test_matrix_initialization()
{
    // Arrange
    Matrix3x5 expected = createSampleMatrix();

    // Act
    Matrix3x5 actual = createSampleMatrix();

    // Assert
    TEST_CHECK(actual == expected);
}

// Test invalid matrix initialization
void test_bad_matrix_initialization()
{
    // Act and Assert
    TEST_EXCEPTION(Matrix3x5 bad_init({0, 1}), std::invalid_argument);
}

// Test matrix assignment
void test_matrix_assignment()
{
    // Arrange
    Matrix3x5 expected = createSampleMatrix();

    // Act
    Matrix3x5 actual = expected;

    // Assert
    TEST_CHECK(actual == expected);
}

// Test matrix move assignment
void test_matrix_move_assignment()
{
    // Arrange
    Matrix3x5 original = createSampleMatrix();
    Matrix3x5 copy = original;

    // Act
    Matrix3x5 moved = std::move(original);

    // Assert
    TEST_CHECK(moved == copy);
}

// Test matrix move constructor
void test_matrix_move_constructor()
{
    // Arrange
    Matrix3x5 original = createSampleMatrix();

    // Act
    Matrix3x5 moved = Matrix3x5(createSampleMatrix());

    // Assert
    TEST_CHECK(moved == original);
}

// Test matrix iteration
void test_matrix_iteration()
{
    // Arrange
    Matrix3x5 matrix = createSampleMatrix();
    std::stringstream ss;

    // Act
    for (auto &value : matrix)
    {
        ss << value << " ";
    }

    // Assert
    auto result = ss.str();
    TEST_CHECK(result == "0 1 2 3 4 5 6 7 8 9 8 7 6 5 4 ");
}

// Test matrix iteration and modification
void test_matrix_iteration_modification()
{
    // Arrange
    Matrix3x5 matrix = createSampleMatrix();

    // Act
    for (auto &value : matrix)
    {
        value = 1;
    }

    // Assert
    Matrix3x5 expected{
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1};
    TEST_CHECK(matrix == expected);
}

// Test matrix iteration out of range error (row)
void test_matrix_iteration_out_of_range_error_row()
{
    // Arrange
    Matrix3x5 matrix = createSampleMatrix();

    // Act and Assert
    TEST_EXCEPTION(matrix[3][1] = 4, std::out_of_range);
}

// Test matrix iteration out of range error (column)
void test_matrix_iteration_out_of_range_error_col()
{
    // Arrange
    Matrix3x5 matrix = createSampleMatrix();

    // Act and Assert
    TEST_EXCEPTION(matrix[1][5] = 4, std::out_of_range);
}

// Define more test cases as needed...

TEST_LIST = {
    {"test_matrix_initialization", test_matrix_initialization},
    {"test_bad_matrix_initialization", test_bad_matrix_initialization},
    {"test_matrix_assignment", test_matrix_assignment},
    {"test_matrix_move_assignment", test_matrix_move_assignment},
    {"test_matrix_move_constructor", test_matrix_move_constructor},
    {"test_matrix_iteration", test_matrix_iteration},
    {"test_matrix_iteration_modification", test_matrix_iteration_modification},
    {"test_matrix_iteration_out_of_range_error_row", test_matrix_iteration_out_of_range_error_row},
    {"test_matrix_iteration_out_of_range_error_col", test_matrix_iteration_out_of_range_error_col},
    // Add more test cases...
};
