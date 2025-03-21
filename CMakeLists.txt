cmake_minimum_required(VERSION 3.20)

# 定义项目名称和版本
project(MyProject VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加src目录作为全局的包含路径
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加src/shared目录
add_subdirectory(src/shared)

# 添加test目录
add_subdirectory(test)