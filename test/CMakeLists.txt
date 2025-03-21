# test/CMakeLists.txt

# 设置当前目录下的可执行文件输出目录为绝对路径
get_filename_component(PROJECT_ROOT ${CMAKE_SOURCE_DIR} ABSOLUTE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/test/bin)

# 如果需要针对不同的构建类型设置相同的输出目录
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_ROOT}/src/bin)
endforeach()

# 定义可执行文件
add_executable(test_queue
        TestQueue.cpp
)

# 寻找并链接共享库
target_link_libraries(test_queue
        PRIVATE
        shared_lib
        pthread
)

# 确保包含shared_lib的头文件搜索路径
target_include_directories(test_queue
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/shared # 根据实际情况调整路径
)