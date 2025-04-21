cmake_minimum_required(VERSION 3.12)
project(HPC_LEARN VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加可执行文件（如果是应用程序）
add_executable(hpc_runner
    src/main.cpp
    src/time_statistic/cal_time.cpp

)

# 设置头文件目录
target_include_directories(hpc_runner PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time_statistic
)

# # 添加测试
# enable_testing()
# add_subdirectory(tests)