################################################################################
# 定义 mutex_lib 库（核心配置）
################################################################################

# 1. 创建名为 mutex_lib 的库，并指定源文件
#
# 功能：将 MutexLock.cpp 编译成一个库（静态库或共享库）。
# 说明：
# - 源文件（.cpp）必须列出，头文件（.hpp）不需要在此处列出。
# - MutexLock.cpp 实现了线程同步功能（如锁、解锁等）。
add_library(mutex_lib
        MutexLock.cpp  # MutexLock 类的实现文件（包含锁的实现）
)

################################################################################
# 设置头文件路径（关键配置）
#
# 问题：其他模块如何找到 mutex_lib 的头文件？
# 解决方案：通过 target_include_directories 告知 CMake 头文件的位置。
################################################################################

# 2. 设置头文件的搜索路径
#
# 功能：告诉 CMake，mutex_lib 的头文件位于当前目录（src/shared/mutex）。
# 参数解释：
# - `PUBLIC`：表示：
#   - mutex_lib 自己需要这些头文件路径（编译自身时用）。
#   - 其他链接 mutex_lib 的目标（如可执行文件）也能使用这些路径。
# - `${CMAKE_CURRENT_SOURCE_DIR}`：指向当前 CMakeLists.txt 文件所在的目录（例如：src/shared/mutex）。
#
# 为什么需要这个？
# - 当其他模块（如 test_queue）使用 mutex_lib 时，需要找到 MutexLock.hpp 头文件。
# - 例如：在 test_queue 中 `#include "MutexLock.hpp"` 需要路径支持。
target_include_directories(mutex_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  # 当前目录即 src/shared/mutex，包含头文件（如 MutexLock.hpp）
)

################################################################################
# 关键点解释（新手必读）
################################################################################

# 🔹 **为什么需要 PUBLIC？**
# - 如果使用 PRIVATE：其他模块链接 mutex_lib 时，看不到这些头文件路径。
#   例如：test_queue 需要包含 MutexLock.hpp 时会报错。
# - 使用 PUBLIC：其他模块可以直接使用这些路径，无需额外配置。

# 🔹 **路径变量解释**
# - `${CMAKE_CURRENT_SOURCE_DIR}`：当前 CMakeLists.txt 文件所在的目录。
#   例如：如果此文件位于 `src/shared/mutex/CMakeLists.txt`，则该变量值为 `src/shared/mutex`。
# - `${CMAKE_SOURCE_DIR}`：项目的根目录（如顶层 `CMakeLists.txt` 所在目录）。

################################################################################
# 验证配置是否正确（新手操作指南）
################################################################################

# 1. 确认头文件路径是否生效：
#   - 在 test_queue 的 CMakeLists.txt 中，链接 mutex_lib 后，可以直接：
#     #include "MutexLock.hpp"  而无需额外设置路径。

# 2. 验证编译是否成功：
#   - 在 test_queue 中使用 MutexLock：
#     MutexLock lock;
#     lock.Lock();  # 如果报错，检查路径是否正确。

# 3. 清理并重新构建：
#   rm -rf build && mkdir build && cd build && cmake .. && make
################################################################################

# 🔥 **常见错误与解决方案**
# 1. 错误：`fatal error: MutexLock.hpp: No such file or directory`
#    - 原因：未设置头文件路径或使用了 PRIVATE 而非 PUBLIC。
#    - 解决：检查 target_include_directories 是否使用了 PUBLIC。

# 2. 错误：`undefined reference to MutexLock::Lock()`
#    - 原因：头文件中声明了方法，但未在源文件中实现。
#    - 解决：确保 MutexLock.cpp 中实现了所有声明的方法。