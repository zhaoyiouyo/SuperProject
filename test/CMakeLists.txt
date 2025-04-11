################################################################################
# 项目根目录设置与输出目录配置
################################################################################

# 获取项目根目录的绝对路径（确保路径正确）
get_filename_component(PROJECT_ROOT ${CMAKE_SOURCE_DIR} ABSOLUTE)

# 设置所有构建类型的可执行文件输出目录为 ${PROJECT_ROOT}/test/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/test/bin)

# 针对不同构建类型（如 Debug/Release）设置相同的输出目录
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PROJECT_ROOT}/test/bin)
endforeach()

################################################################################
# 定义可执行文件 test_queue
################################################################################
add_executable(test_queue
        TestQueue.cpp
)

################################################################################
# 链接库配置（关键！必须遵循符号解析规则）
#
# 问题：链接器按顺序扫描库，无法回退！
# 如果 A 依赖 B，B 必须出现在 A 之后，否则符号未定义错误（如 MutexLock::lock() 未定义）
#
# 解决方案：
# 1. 显式声明子库的依赖关系（如 job_lib 必须在自己的 CMake 文件中链接 mutex_lib）
# 2. 主可执行文件的链接顺序：将提供符号的库（如 mutex_lib）放在所有依赖它的库之后
# 3. 可选：使用组选项（-Wl,--start-group ... --end-group）强制链接器多次扫描
################################################################################

# 方案 1：显式声明子库的依赖关系（必须在子库的 CMake 文件中完成）
# 示例：在 job_lib 的 CMake 文件中：
# target_link_libraries(job_lib PRIVATE mutex_lib)
# 在 queue_lib 的 CMake 文件中：
# target_link_libraries(queue_lib INTERFACE mutex_lib)

# 方案 2：主可执行文件的链接顺序（正确顺序）
target_link_libraries(test_queue
        PRIVATE
        job_lib     # 依赖 mutex_lib（必须先声明）
        queue_lib   # 依赖 mutex_lib（必须先声明）
        mutex_lib   # 提供符号（必须放在最后！）
        pthread     # 系统库放在最后
)

# 方案 3：使用组选项（可选，解决复杂依赖）
# target_link_libraries(test_queue
#     PRIVATE
#     -Wl,--start-group
#     job_lib
#     queue_lib
#     mutex_lib
#     -Wl,--end-group
#     pthread
# )

################################################################################
# 头文件路径配置
#
# 注意：
# 1. 子库（如 job_lib）必须在自己的 CMake 文件中设置头文件路径（如 mutex 的头文件路径）
# 2. 主可执行文件的头文件路径仅用于自身，不影响子库
################################################################################
target_include_directories(test_queue
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/shared/mutex  # MutexLock.hpp 的路径
        ${CMAKE_SOURCE_DIR}/src/shared/queue  # BaseQueue.hpp 的路径
        ${CMAKE_SOURCE_DIR}/src/job           # JobQueue.cpp 的路径
)

################################################################################
# 关键问题与解决方案总结（快速查阅）
################################################################################

# 问题：符号未定义（如 "MutexLock::lock() 未定义"）
# 原因：
# 1. 子库（如 job_lib）未显式链接 mutex_lib
# 2. 主可执行文件的链接顺序错误（如 mutex_lib 在 job_lib 之前）

# 解决步骤：
# 1. 在子库的 CMake 文件中声明依赖：
#    target_link_libraries(job_lib PRIVATE mutex_lib)
#    target_link_libraries(queue_lib PRIVATE mutex_lib)

# 2. 主可执行文件的链接顺序：
#    job_lib → queue_lib → mutex_lib（依赖库放在前面，提供符号的库放在最后）

# 3. 验证链接命令：
#    make VERBOSE=1 | grep "g++"
#    确保输出为：... -ljob_lib -lqueue_lib -lmutex_lib ...

# 4. 如果依赖关系复杂，使用组选项：
#    -Wl,--start-group ... -Wl,--end-group
################################################################################

# 清理并重新构建（解决缓存问题）
# rm -rf build && mkdir build && cd build && cmake .. && make