#include <iostream>
#include "cal_time.h"
#include <thread> // 包含此头文件以使用 std::this_thread::sleep_for
#include <chrono> // 包含此头文件以使用 chrono 库
int main()
{
    TimeLabel time_label;
    time_label.compute();
    time_label.print_time();
    // std::cout<<"Hello World!"<<std::endl;
    time_label.reInit();
    // int b = 1;
    // for(auto i=1; i< 10000000; i++)
    // {
    //     b++;
    // }
    // std::cout << "Current date and time: " << b << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    time_label.compute();
    time_label.print_time();

    return 0;
}