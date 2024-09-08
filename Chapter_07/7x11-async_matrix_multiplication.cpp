#include <cmath>
#include <exception>
#include <future>
#include <iostream>
#include <vector>

using matrix_t = std::vector<std::vector<int>>;

int dot_product(const std::vector<int>& a, const std::vector<int>& b) {
    int sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

// Function to perform matrix multiplication
matrix_t matrix_multiply(const matrix_t& A, const matrix_t& B) {
    if (A[0].size() != B.size()) {
        throw new std::runtime_error("Wrong matrices dimmensions.");
    }
    std::size_t rows = A.size();
    std::size_t cols = B[0].size();
    std::size_t inner_dim = B.size();
    matrix_t res(rows, std::vector<int>(cols, 0));

    // Launch one task when multiplying row from A and column from B
    std::vector<std::future<int>> futs;
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            // Extract column j from B
            std::vector<int> column(inner_dim);
            for (std::size_t k = 0; k < inner_dim; ++k) {
                column[k] = B[k][j];
            }
            // Multiply row i from A and column j from B
            futs.push_back(std::async(std::launch::async, dot_product, A[i], column));
        }
    }

    // Collect results
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            res[i][j] = futs[i * cols + j].get();
        }
    }
    return res;
}

int main() {
    auto show_matrix = [](const std::string& name, matrix_t& mtx) {
        std::cout << name << '\n';
        for (const auto& row : mtx) {
            for (const auto& elem : row) {
                std::cout << elem << " ";
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    };

    matrix_t A = {{1, 2, 3}, 
                  {4, 5, 6}};

    matrix_t B = {{7, 8, 9}, 
                  {10, 11, 12}, 
                  {13, 14, 15}};

    auto res = matrix_multiply(A, B);

    show_matrix("A", A);
    show_matrix("B", B);
    show_matrix("Result", res);

    return 0;
}
