#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <algorithm>


    class Matrix {
    private:
        std::vector<std::vector<double>> data;
        size_t rows;
        size_t cols;

    public:
        // 构造函数
        Matrix(const std::vector<std::vector<double>>& input) : data(input), rows(input.size()), cols(input.empty() ? 0 : input[0].size()) {
            for (const auto& row : data) {
                if (row.size() != cols) {
                    throw std::invalid_argument("所有行必须具有相同的列数");
                }
            }
        }

        // 构造函数（零矩阵）
        Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
            data.resize(rows, std::vector<double>(cols, 0.0));
        }

        // 拷贝构造函数
        Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}

        // 读行数
        size_t get_rows() const {
            return rows;
        }

        // 读列数
        size_t get_cols() const {
            return cols;
        }

        // 读元素1
        double& operator()(size_t i, size_t j) {
            if (i >= rows || j >= cols) {
                throw std::out_of_range("矩阵索引越界");
            }
            return data[i][j];
        }

        // 读元素2（只读！）
        double operator()(size_t i, size_t j) const {
            if (i >= rows || j >= cols) {
                throw std::out_of_range("矩阵索引越界");
            }
            return data[i][j];
        }

        // 更新元素
        void update(size_t i, size_t j, double new_value) {
            if (i >= rows || j >= cols) {
                throw std::out_of_range("矩阵索引越界");
            }
            data[i][j] = new_value;
        }
        // 矩阵加法
        Matrix operator+(const Matrix& other) const {
            if (rows != other.rows || cols != other.cols) {
                throw std::invalid_argument("矩阵维度不匹配，无法相加");
            }

            Matrix result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(i, j) = data[i][j] + other(i, j);
                }
            }
            return result;
        }

        // 矩阵减法
        Matrix operator-(const Matrix& other) const {
            if (rows != other.rows || cols != other.cols) {
                throw std::invalid_argument("矩阵维度不匹配，无法相减");
            }

            Matrix result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(i, j) = data[i][j] - other(i, j);
                }
            }
            return result;
        }

        // 矩阵乘法
        Matrix operator*(const Matrix& other) const {
            if (cols != other.rows) {
                throw std::invalid_argument("矩阵维度不匹配，无法相乘");
            }

            Matrix result(rows, other.cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < other.cols; ++j) {
                    for (size_t k = 0; k < cols; ++k) {
                        result(i, j) += data[i][k] * other(k, j);
                    }
                }
            }
            return result;
        }

        // 向量右乘矩阵
        std::vector<double> operator*(std::vector<double> vec) const {
            if (cols != vec.size()) {
                throw std::invalid_argument("矩阵向量维度不匹配，无法相乘");
            }

            std::vector<double> result(rows, 0.0);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < vec.size(); j++) {
                    result[i] += data[i][j] * vec[j];
                }
            }
            return result;
        }

        // 标量乘法
        Matrix operator*(double scalar) const {
            Matrix result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(i, j) = data[i][j] * scalar;
                }
            }
            return result;
        }

        // 标量左乘
        friend Matrix operator*(double scalar, const Matrix& matrix) {
            return matrix * scalar;
        }

        // 矩阵转置
        Matrix transpose() const {
            Matrix result(cols, rows);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(j, i) = data[i][j];
                }
            }
            return result;
        }

        // 判断矩阵是否为单位矩阵
        bool is_identity() const {
            if (rows != cols) {
                return false;
            }
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    if (i == j) {
                        if (std::abs(data[i][j] - 1.0) > 1e-10) {
                            return false;
                        }
                    }
                    else {
                        if (std::abs(data[i][j]) > 1e-10) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        // 输出矩阵
        friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
            for (size_t i = 0; i < matrix.rows; ++i) {
                for (size_t j = 0; j < matrix.cols; ++j) {
                    os << std::setw(10) << std::fixed << std::setprecision(4) << matrix(i, j);
                }
                os << std::endl;
            }
            return os;
        }

    };

