################################################################################
# 定义 job_lib 库（核心配置）
################################################################################

# 1. 创建名为 job_lib 的库，并指定源文件
#
# 功能：将 JobManager.cpp 和 JobQueue.cpp 编译成一个库（静态库或共享库）。
# 说明：
# - 源文件（.cpp）必须列出，头文件（.hpp）不需要在此处列出。
# - 这两个文件实现了 JobManager 和 JobQueue 类的功能。
add_library(job_lib
        JobManager.cpp     # JobManager 类的实现文件
        JobQueue.cpp       # JobQueue 类的实现文件
)

################################################################################
# 设置头文件路径（关键配置）
#
# 问题：其他模块如何找到 job_lib 的头文件？
# 解决方案：通过 target_include_directories 告知 CMake 头文件的位置。
################################################################################

# 2. 设置头文件的搜索路径
#
# 功能：告诉 CMake，job_lib 的头文件位于当前目录（src/job）。
# 参数解释：
# - `PUBLIC`：表示：
#   - job_lib 自己需要这些头文件路径（编译自身时用）。
#   - 其他链接 job_lib 的目标（如可执行文件）也能使用这些路径。
# - `${CMAKE_CURRENT_SOURCE_DIR}`：指向当前 CMakeLists.txt 文件所在的目录（例如：src/job）。
#
# 为什么需要这个？
# - 当其他模块（如 test_queue）使用 job_lib 时，它们需要找到 JobManager.hpp 或 JobQueue.hpp。
# - 特别地，如果 JobQueue 是一个模板类（如 `template<class T> class JobQueue`），
#   它的定义必须在头文件中，否则其他模块无法正确实例化模板。
target_include_directories(job_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  # 当前目录即 src/job，包含头文件（如 JobManager.hpp）
)

################################################################################
# 关键点解释（新手必读）
################################################################################

# 🔹 **为什么需要 PUBLIC？**
# - 如果使用 PRIVATE：其他模块链接 job_lib 时，看不到这些头文件路径。
#   例如：test_queue 需要包含 JobManager.hpp 时会报错。
# - 使用 PUBLIC：其他模块可以直接使用这些路径，无需额外配置。

# 🔹 **模板类的特殊性**
# - 如果 JobQueue 是模板类（如 `template<class T> class JobQueue`）：
#   - 其定义必须完全在头文件中（如 JobQueue.hpp）。
#   - 因此，其他模块必须能访问到 JobQueue.hpp，否则编译失败。
#   - 通过设置 PUBLIC 的头文件路径，确保其他模块能找到模板定义。

# 🔹 **路径变量解释**
# - `${CMAKE_CURRENT_SOURCE_DIR}`：当前 CMakeLists.txt 文件所在的目录。
#   例如：如果此文件位于 `src/job/CMakeLists.txt`，则该变量值为 `src/job`。
# - `${CMAKE_SOURCE_DIR}`：项目的根目录（如项目顶层的 `CMakeLists.txt` 所在目录）。

################################################################################
# 验证配置是否正确（新手操作指南）
################################################################################

# 1. 确认头文件路径是否生效：
#   - 在 test_queue 的 CMakeLists.txt 中，链接 job_lib 后，可以直接：
#     #include "JobManager.hpp"  而无需额外设置路径。

# 2. 验证模板类是否可见：
#   - 在 test_queue 中使用模板类：
#     JobQueue<int> myQueue;  # 如果报错，检查路径是否正确。

# 3. 清理并重新构建：
#   rm -rf build && mkdir build && cd build && cmake .. && make
################################################################################

# 🔥 **常见错误与解决方案**
# 1. 错误：`fatal error: JobManager.hpp: No such file or directory`
#    - 原因：未设置头文件路径或使用了 PRIVATE 而非 PUBLIC。
#    - 解决：检查 target_include_directories 是否使用了 PUBLIC。

# 2. 错误：`undefined reference to template instantiation ‘JobQueue<int>’`
#    - 原因：模板类的定义未在头文件中提供。
#    - 解决：确保模板类的完整定义在头文件中（如 JobQueue.hpp）。