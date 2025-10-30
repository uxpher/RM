cmake_minimum_required(VERSION 3.10)
project(simulator)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加源文件
add_executable(simulator
    simulator.cpp
)

