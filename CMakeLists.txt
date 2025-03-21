# 设置CMake的最低版本要求为3.20
# 这确保了CMake具备所需的特性来处理这个项目的构建配置
cmake_minimum_required(VERSION 3.20)

# 定义项目名称和版本
# `project`命令不仅定义了项目的名称（MyProject），还指定了其版本号（1.0）
project(MyProject VERSION 1.0)

# 设置C++标准为C++17，并要求严格遵守该标准
# `set`命令用于设置变量值，这里设置了C++标准和要求严格遵守该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加src目录作为全局的包含路径
# `include_directories`命令将指定的目录添加到编译器的头文件搜索路径中
# 这使得在src目录下的所有头文件都可以被直接引用，无需使用相对路径
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加src/shared子目录
# `add_subdirectory`命令告诉CMake去指定的子目录下查找并处理CMakeLists.txt文件
# 这样可以将构建逻辑分模块化，便于管理和维护
add_subdirectory(src/shared)

# 添加test子目录
# 类似地，这个命令指示CMake去处理test目录下的构建逻辑
# 通常用于组织单元测试代码或其他测试相关的构建配置
add_subdirectory(test)