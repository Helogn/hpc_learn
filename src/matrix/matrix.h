#ifndef MATRIX_COMPUTATIONS_H
#define MATRIX_COMPUTATIONS_H

#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <functional>
#include <array>
#include <cassert>

// 手动实现的矩阵类
class ManualMatrix {
public:
    ManualMatrix(size_t rows, size_t cols);
    ManualMatrix(size_t rows, size_t cols, double initValue);
    ManualMatrix(const std::vector<std::vector<double>>& data);
    ManualMatrix(const ManualMatrix& other);
    ManualMatrix(ManualMatrix&& other) noexcept;
    
    ~ManualMatrix() = default;
    
    ManualMatrix& operator=(const ManualMatrix& other);
    ManualMatrix& operator=(ManualMatrix&& other) noexcept;
    
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;
    
    ManualMatrix operator+(const ManualMatrix& other) const;
    ManualMatrix operator-(const ManualMatrix& other) const;
    ManualMatrix operator*(const ManualMatrix& other) const;
    ManualMatrix operator*(double scalar) const;
    
    ManualMatrix transpose() const;
    double determinant() const;
    ManualMatrix inverse() const;
    
    static ManualMatrix identity(size_t size);
    
    void print(std::ostream& os = std::cout) const;
    
private:
    size_t m_rows;
    size_t m_cols;
    std::vector<double> m_data;
    
    size_t index(size_t row, size_t col) const {
        return row * m_cols + col;
    }
    
    double minor(size_t row, size_t col) const;
};

// 使用标准库实现的矩阵运算
namespace StdMatrix {
    using Matrix = std::vector<std::vector<double>>;
    
    Matrix create(size_t rows, size_t cols, double initValue = 0.0);
    Matrix identity(size_t size);
    
    Matrix add(const Matrix& a, const Matrix& b);
    Matrix subtract(const Matrix& a, const Matrix& b);
    Matrix multiply(const Matrix& a, const Matrix& b);
    Matrix multiply(const Matrix& a, double scalar);
    
    Matrix transpose(const Matrix& m);
    double determinant(const Matrix& m);
    Matrix inverse(const Matrix& m);
    
    void print(const Matrix& m, std::ostream& os = std::cout);
}

// 使用高性能库(Eigen)的矩阵运算
#ifdef USE_EIGEN
#include <Eigen/Dense>

namespace EigenMatrix {
    using Matrix = Eigen::MatrixXd;
    
    Matrix create(size_t rows, size_t cols, double initValue = 0.0);
    Matrix identity(size_t size);
    
    Matrix add(const Matrix& a, const Matrix& b);
    Matrix subtract(const Matrix& a, const Matrix& b);
    Matrix multiply(const Matrix& a, const Matrix& b);
    Matrix multiply(const Matrix& a, double scalar);
    
    Matrix transpose(const Matrix& m);
    double determinant(const Matrix& m);
    Matrix inverse(const Matrix& m);
    
    void print(const Matrix& m, std::ostream& os = std::cout);
}
#endif // USE_EIGEN

#endif // MATRIX_COMPUTATIONS_H