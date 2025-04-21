#include <iostream>
#include "cal_time.h"
#include "matrix.h"
#include <thread> // 包含此头文件以使用 std::this_thread::sleep_for
#include <chrono> // 包含此头文件以使用 chrono 库
int main()
{
    TimeLabel time_label;
    time_label.compute();
    time_label.print_time();

    // INIT VAL
    int row = 1000, col = 1000, value = 15;

    // test matrix init
    ManualMatrix matrixA = ManualMatrix(row,col,value);

    time_label.reInit();
    ManualMatrix matrixRes = matrixA * matrixA;

    time_label.compute();
    time_label.print_time("Manual: ");

    // 使用标准库
    StdMatrix::Matrix MatrixB = StdMatrix::create(row, col, value);
    time_label.reInit();
    StdMatrix::Matrix MatrixBRes = StdMatrix::multiply(MatrixB, MatrixB);
    time_label.compute();
    time_label.print_time("standard: ");

    // 使用Eigen计算
    time_label.reInit();
    EigenMatrix::Matrix MatrixC = EigenMatrix::create(row, col, value);
    EigenMatrix::Matrix MatrixCRes = EigenMatrix::multiply(MatrixC, MatrixC);
    time_label.compute();
    time_label.print_time("Eigen: ");

    return 0;
}