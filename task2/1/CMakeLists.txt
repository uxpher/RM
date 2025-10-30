# 设置所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(ImageProcessorProject)


find_package(OpenCV REQUIRED)

# OpenCV 版本信息 
message(STATUS "OpenCV library status:")
message(STATUS "    Version: ${OpenCV_VERSION}")
message(STATUS "    Includes: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    Libraries: ${OpenCV_LIBS}")

# 定义源文件
set(SOURCE_FILES
    code/segmentation.cpp
)

# 创建可执行文件
add_executable(image_processor ${SOURCE_FILES})

# 链接 OpenCV 库到可执行文件
target_link_libraries(image_processor ${OpenCV_LIBS})

# 添加头文件搜索路径
target_include_directories(image_processor PUBLIC
    code
    ${OpenCV_INCLUDE_DIRS}
)

# 复制所需的图像文件到构建目录
file(COPY test_image.jpg DESTINATION ${CMAKE_BINARY_DIR})
file(COPY seg.jpg DESTINATION ${CMAKE_BINARY_DIR})
