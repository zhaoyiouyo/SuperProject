# 设置当前目录下的可执行文件输出目录为绝对路径
# 使用 get_filename_component 获取项目的根目录，并设置为绝对路径
get_filename_component(PROJECT_ROOT ${CMAKE_SOURCE_DIR} ABSOLUTE)

# 设置所有构建类型的可执行文件输出目录为 ${PROJECT_ROOT}/test/bin
# 这确保了生成的可执行文件将被放置在指定的输出目录中
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/test/bin)

# 如果需要针对不同的构建类型（如 Debug, Release）设置相同的输出目录
# 遍历所有可能的构建配置类型，并为每种类型设置相同的输出目录
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    # 将构建类型名称转换为大写形式
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)

    # 为每个构建类型设置特定的输出目录
    # 注意：这里的目标目录是 ${PROJECT_ROOT}/src/bin，可能需要根据实际需求调整
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_ROOT}/src/bin)
endforeach()

# 定义可执行文件 test_queue
# 指定源文件 TestQueue.cpp 来构建名为 test_queue 的可执行文件
add_executable(test_queue
        TestQueue.cpp
)

# 寻找并链接共享库 shared_lib 和系统库 pthread
# PRIVATE 关键字表示这些库仅在链接 test_queue 时使用，不会传播给依赖它的其他目标
target_link_libraries(test_queue
        PRIVATE
        shared_lib  # 假设这是一个自定义的共享库
        pthread     # POSIX 线程库，用于多线程编程支持
)

# 确保包含 shared_lib 的头文件搜索路径
# PRIVATE 关键字表示这些包含路径仅对 test_queue 目标有效
target_include_directories(test_queue
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/shared  # 根据实际情况调整路径，指向 shared 库的头文件目录
)