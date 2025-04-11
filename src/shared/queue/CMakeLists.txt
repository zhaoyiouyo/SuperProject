################################################################################
# 定义 queue_lib 库（接口库，仅传递头文件路径）
################################################################################

# 1. 创建名为 queue_lib 的接口库（不包含源文件）
#
# 功能：声明一个接口库，用于传递头文件路径和编译选项。
# 说明：
# - 接口库（`INTERFACE`）不生成实际的库文件（如 .a 或 .so）。
# - 它的作用是：
#   - 将头文件路径暴露给其他模块。
#   - 传递编译选项（如宏定义、其他依赖）。
# - 适用于纯头文件库（如模板类、接口定义）。
add_library(queue_lib INTERFACE)  # 注意：类型为 INTERFACE

################################################################################
# 设置头文件路径（关键配置）
#
# 问题：其他模块如何找到 queue_lib 的头文件？
# 解决方案：通过 `target_include_directories` 告知 CMake 头文件的位置。
################################################################################

# 2. 设置头文件的搜索路径
#
# 功能：告诉 CMake，queue_lib 的头文件位于当前目录（src/shared/queue）。
# 参数解释：
# - `INTERFACE`：表示：
#   - 该头文件路径仅对依赖 queue_lib 的目标（如可执行文件）有效。
#   - 接口库本身不需要编译，因此无需设置 `PUBLIC` 或 `PRIVATE`。
# - `${CMAKE_CURRENT_SOURCE_DIR}`：指向当前 CMakeLists.txt 文件所在的目录（例如：src/shared/queue）。
#
# 为什么需要这个？
# - 当其他模块（如 test_queue）使用 queue_lib 时，需要找到 BaseQueue.hpp 头文件。
# - 例如：在 test_queue 中 `#include "BaseQueue.hpp"` 需要路径支持。
target_include_directories(queue_lib
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}  # 当前目录即 src/shared/queue，包含头文件（如 BaseQueue.hpp）
)

################################################################################
# 关键点解释（新手必读）
################################################################################

# 🔹 **为什么使用 INTERFACE 库？**
# - queue_lib 是一个纯头文件库（如模板类 BaseQueue 的定义在头文件中）。
# - 不需要编译源文件，只需传递头文件路径和编译选项。

# 🔹 **为什么使用 INTERFACE 作用域？**
# - 接口库的属性（如头文件路径）仅对依赖它的目标生效。
# - 如果使用 `PUBLIC` 或 `PRIVATE`，会导致警告或错误，因为接口库本身无需编译。

# 🔹 **路径变量解释**
# - `${CMAKE_CURRENT_SOURCE_DIR}`：当前 CMakeLists.txt 文件所在的目录。
#   例如：如果此文件位于 `src/shared/queue/CMakeLists.txt`，则该变量值为 `src/shared/queue`。
# - `${CMAKE_SOURCE_DIR}`：项目的根目录（如顶层 `CMakeLists.txt` 所在目录）。

################################################################################
# 验证配置是否正确（新手操作指南）
################################################################################

# 1. 确认头文件路径是否生效：
#   - 在 test_queue 的 CMakeLists.txt 中，链接 queue_lib 后，可以直接：
#     #include "BaseQueue.hpp"  而无需额外设置路径。

# 2. 验证编译是否成功：
#   - 在 test_queue 中使用 BaseQueue：
#     BaseQueue<int> myQueue;  # 如果报错，检查路径是否正确。

# 3. 清理并重新构建：
#   rm -rf build && mkdir build && cd build && cmake .. && make
################################################################################

# 🔥 **常见错误与解决方案**
# 1. 错误：`fatal error: BaseQueue.hpp: No such file or directory`
#    - 原因：未设置头文件路径或作用域使用错误（如误用 PUBLIC）。
#    - 解决：确保 target_include_directories 使用 `INTERFACE`。

# 2. 错误：`queue_lib` 是接口库，无法链接源文件
#    - 原因：尝试在 add_library 中列出 .cpp 文件。
#    - 解决：接口库不包含源文件，仅传递头文件路径。

# 3. 错误：`undefined reference to BaseQueue::Method()`
#    - 原因：BaseQueue 是模板类，但未在头文件中实现。
#    - 解决：确保模板类的定义完全在头文件中（如 BaseQueue.hpp）。