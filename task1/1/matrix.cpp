#include "Matrix.h"
int main() {
    // 创建矩阵
    Matrix A({ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} });
    Matrix B({ {1.1,2.2,3.3}, {4.4,5.5,6.6}, {7.7,8.8,9.9} });

    std::cout << "矩阵 A:" << std::endl;
    std::cout << A << std::endl;

    std::cout << "矩阵 B:" << std::endl;
    std::cout << B << std::endl;

    // 矩阵加法
    Matrix C = A + B;
    std::cout << "A + B:" << std::endl;
    std::cout << C << std::endl;

    // 矩阵乘法
    Matrix D = A * B;
    std::cout << "A * B:" << std::endl;
    std::cout << D << std::endl;

    // 标量乘法
    Matrix E = 2.5 * A;
    std::cout << "2.5 * A:" << std::endl;
    std::cout << E << std::endl;

    // 矩阵转置
    Matrix F = A.transpose();
    std::cout << "A 的转置:" << std::endl;
    std::cout << F << std::endl;

    // 更新矩阵
    A.update(0, 0, 5.5);
    std::cout << "更新后的矩阵 A:" << std::endl;
    std::cout << A << std::endl;
    return 0;
}