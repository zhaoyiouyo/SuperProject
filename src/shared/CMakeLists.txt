# src/shared/CMakeLists.txt

# 定义库名
add_library(shared_lib
        BaseQueue.hpp
        TapeDrivesQueue.hpp
        TapeDrivesQueue.cpp
        TapeDrivesOperation.hpp
        TapeDrivesOperation.cpp
        MutexLock.cpp
        NonCopyable.hpp
)

# 设置包含目录，以便其他模块可以找到头文件
# 使用 PUBLIC 关键字表示这个包含路径不仅对 shared_lib 自己有效，
# 对所有链接到 shared_lib 的目标也有效。
target_include_directories(shared_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} # 当前目录包含了shared.hpp
)