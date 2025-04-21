#include "matrix.h"
#include <cmath>
#include <iomanip>

// ==================== 手动实现的矩阵类 ====================

ManualMatrix::ManualMatrix(size_t rows, size_t cols) 
    : m_rows(rows), m_cols(cols), m_data(rows * cols, 0.0) {}

ManualMatrix::ManualMatrix(size_t rows, size_t cols, double initValue)
    : m_rows(rows), m_cols(cols), m_data(rows * cols, initValue) {}

ManualMatrix::ManualMatrix(const std::vector<std::vector<double>>& data) {
    if (data.empty() || data[0].empty()) {
        throw std::invalid_argument("Invalid matrix dimensions");
    }
    
    m_rows = data.size();
    m_cols = data[0].size();
    m_data.reserve(m_rows * m_cols);
    
    for (const auto& row : data) {
        if (row.size() != m_cols) {
            throw std::invalid_argument("Inconsistent row sizes");
        }
        m_data.insert(m_data.end(), row.begin(), row.end());
    }
}

ManualMatrix::ManualMatrix(const ManualMatrix& other)
    : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data) {}

ManualMatrix::ManualMatrix(ManualMatrix&& other) noexcept
    : m_rows(other.m_rows), m_cols(other.m_cols), m_data(std::move(other.m_data)) {
    other.m_rows = 0;
    other.m_cols = 0;
}

ManualMatrix& ManualMatrix::operator=(const ManualMatrix& other) {
    if (this != &other) {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = other.m_data;
    }
    return *this;
}

ManualMatrix& ManualMatrix::operator=(ManualMatrix&& other) noexcept {
    if (this != &other) {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = std::move(other.m_data);
        
        other.m_rows = 0;
        other.m_cols = 0;
    }
    return *this;
}

double& ManualMatrix::operator()(size_t row, size_t col) {
    if (row >= m_rows || col >= m_cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return m_data[index(row, col)];
}

const double& ManualMatrix::operator()(size_t row, size_t col) const {
    if (row >= m_rows || col >= m_cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return m_data[index(row, col)];
}

ManualMatrix ManualMatrix::operator+(const ManualMatrix& other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions mismatch for addition");
    }
    
    ManualMatrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_data.size(); ++i) {
        result.m_data[i] = m_data[i] + other.m_data[i];
    }
    return result;
}

ManualMatrix ManualMatrix::operator-(const ManualMatrix& other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions mismatch for subtraction");
    }
    
    ManualMatrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_data.size(); ++i) {
        result.m_data[i] = m_data[i] - other.m_data[i];
    }
    return result;
}

ManualMatrix ManualMatrix::operator*(const ManualMatrix& other) const {
    if (m_cols != other.m_rows) {
        throw std::invalid_argument("Matrix dimensions mismatch for multiplication");
    }
    
    ManualMatrix result(m_rows, other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < m_cols; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

ManualMatrix ManualMatrix::operator*(double scalar) const {
    ManualMatrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_data.size(); ++i) {
        result.m_data[i] = m_data[i] * scalar;
    }
    return result;
}

ManualMatrix ManualMatrix::transpose() const {
    ManualMatrix result(m_cols, m_rows);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

double ManualMatrix::minor(size_t row, size_t col) const {
    ManualMatrix submatrix(m_rows - 1, m_cols - 1);
    
    size_t sub_i = 0;
    for (size_t i = 0; i < m_rows; ++i) {
        if (i == row) continue;
        
        size_t sub_j = 0;
        for (size_t j = 0; j < m_cols; ++j) {
            if (j == col) continue;
            
            submatrix(sub_i, sub_j) = (*this)(i, j);
            ++sub_j;
        }
        ++sub_i;
    }
    
    return submatrix.determinant();
}

double ManualMatrix::determinant() const {
    if (m_rows != m_cols) {
        throw std::logic_error("Determinant can only be calculated for square matrices");
    }
    
    if (m_rows == 1) {
        return m_data[0];
    }
    
    if (m_rows == 2) {
        return m_data[0] * m_data[3] - m_data[1] * m_data[2];
    }
    
    double det = 0.0;
    for (size_t j = 0; j < m_cols; ++j) {
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        det += sign * (*this)(0, j) * this->minor(0, j);
    }
    
    return det;
}

ManualMatrix ManualMatrix::inverse() const {
    if (m_rows != m_cols) {
        throw std::logic_error("Inverse can only be calculated for square matrices");
    }
    
    double det = determinant();
    if (std::abs(det) < 1e-10) {
        throw std::logic_error("Matrix is singular (determinant is zero)");
    }
    
    ManualMatrix inv(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            inv(j, i) = sign * minor(i, j) / det; // Note the transpose here
        }
    }
    
    return inv;
}

ManualMatrix ManualMatrix::identity(size_t size) {
    ManualMatrix result(size, size);
    for (size_t i = 0; i < size; ++i) {
        result(i, i) = 1.0;
    }
    return result;
}

void ManualMatrix::print(std::ostream& os) const {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            os << std::setw(10) << (*this)(i, j) << " ";
        }
        os << "\n";
    }
}

// ==================== 标准库实现的矩阵运算 ====================

namespace StdMatrix {

Matrix create(size_t rows, size_t cols, double initValue) {
    return Matrix(rows, std::vector<double>(cols, initValue));
}

Matrix identity(size_t size) {
    Matrix result = create(size, size, 0.0);
    for (size_t i = 0; i < size; ++i) {
        result[i][i] = 1.0;
    }
    return result;
}

Matrix add(const Matrix& a, const Matrix& b) {
    if (a.size() != b.size() || a.empty() || a[0].size() != b[0].size()) {
        throw std::invalid_argument("Matrix dimensions mismatch for addition");
    }
    
    Matrix result = create(a.size(), a[0].size());
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < a[0].size(); ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

Matrix subtract(const Matrix& a, const Matrix& b) {
    if (a.size() != b.size() || a.empty() || a[0].size() != b[0].size()) {
        throw std::invalid_argument("Matrix dimensions mismatch for subtraction");
    }
    
    Matrix result = create(a.size(), a[0].size());
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < a[0].size(); ++j) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

Matrix multiply(const Matrix& a, const Matrix& b) {
    if (a.empty() || b.empty() || a[0].size() != b.size()) {
        throw std::invalid_argument("Matrix dimensions mismatch for multiplication");
    }
    
    Matrix result = create(a.size(), b[0].size());
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b[0].size(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a[0].size(); ++k) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

Matrix multiply(const Matrix& a, double scalar) {
    Matrix result = create(a.size(), a[0].size());
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < a[0].size(); ++j) {
            result[i][j] = a[i][j] * scalar;
        }
    }
    return result;
}

Matrix transpose(const Matrix& m) {
    if (m.empty()) return Matrix();
    
    Matrix result = create(m[0].size(), m.size());
    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m[0].size(); ++j) {
            result[j][i] = m[i][j];
        }
    }
    return result;
}

double determinant(const Matrix& m) {
    if (m.size() != m[0].size()) {
        throw std::logic_error("Determinant can only be calculated for square matrices");
    }
    
    if (m.size() == 1) {
        return m[0][0];
    }
    
    if (m.size() == 2) {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
    
    double det = 0.0;
    for (size_t j = 0; j < m[0].size(); ++j) {
        Matrix submatrix(m.size() - 1, std::vector<double>(m.size() - 1));
        
        for (size_t i = 1; i < m.size(); ++i) {
            size_t sub_j = 0;
            for (size_t k = 0; k < m[0].size(); ++k) {
                if (k == j) continue;
                submatrix[i-1][sub_j++] = m[i][k];
            }
        }
        
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        det += sign * m[0][j] * determinant(submatrix);
    }
    
    return det;
}

Matrix inverse(const Matrix& m) {
    if (m.size() != m[0].size()) {
        throw std::logic_error("Inverse can only be calculated for square matrices");
    }
    
    double det = determinant(m);
    if (std::abs(det) < 1e-10) {
        throw std::logic_error("Matrix is singular (determinant is zero)");
    }
    
    Matrix inv(m.size(), std::vector<double>(m.size()));
    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m.size(); ++j) {
            Matrix minorMatrix(m.size() - 1, std::vector<double>(m.size() - 1));
            
            size_t minor_i = 0;
            for (size_t k = 0; k < m.size(); ++k) {
                if (k == i) continue;
                
                size_t minor_j = 0;
                for (size_t l = 0; l < m.size(); ++l) {
                    if (l == j) continue;
                    minorMatrix[minor_i][minor_j++] = m[k][l];
                }
                ++minor_i;
            }
            
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            inv[j][i] = sign * determinant(minorMatrix) / det; // Note the transpose here
        }
    }
    
    return inv;
}

void print(const Matrix& m, std::ostream& os) {
    for (const auto& row : m) {
        for (double val : row) {
            os << std::setw(10) << val << " ";
        }
        os << "\n";
    }
}

} // namespace StdMatrix

// ==================== Eigen库实现的矩阵运算 ====================

#ifdef USE_EIGEN
namespace EigenMatrix {

Matrix create(size_t rows, size_t cols, double initValue) {
    Matrix m(rows, cols);
    m.setConstant(initValue);
    return m;
}

Matrix identity(size_t size) {
    return Matrix::Identity(size, size);
}

Matrix add(const Matrix& a, const Matrix& b) {
    return a + b;
}

Matrix subtract(const Matrix& a, const Matrix& b) {
    return a - b;
}

Matrix multiply(const Matrix& a, const Matrix& b) {
    return a * b;
}

Matrix multiply(const Matrix& a, double scalar) {
    return a * scalar;
}

Matrix transpose(const Matrix& m) {
    return m.transpose();
}

double determinant(const Matrix& m) {
    return m.determinant();
}

Matrix inverse(const Matrix& m) {
    return m.inverse();
}

void print(const Matrix& m, std::ostream& os) {
    os << m << "\n";
}

} // namespace EigenMatrix
#endif // USE_EIGEN