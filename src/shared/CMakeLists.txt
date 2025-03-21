# 定义库名 shared_lib，并指定源文件和头文件
# 使用 add_library 命令创建一个名为 shared_lib 的库
# 注意：这里不仅包含了 .cpp 文件，还包含了 .hpp 文件，
# 这是因为 BaseQueue 是一个模板类，其定义必须在头文件中完全可见，
# 以便在使用该模板类的任何地方都能正确实例化。
add_library(shared_lib
        BaseQueue.hpp           # 模板类定义，必须包含以确保可实例化
        TapeDrivesQueue.cpp     # 类实现
        TapeDrivesOperation.cpp # 类实现
        MutexLock.cpp           # 类实现
)

# 设置包含目录，以便其他模块可以找到头文件
# 使用 PUBLIC 关键字表示这个包含路径不仅对 shared_lib 自己有效，
# 对所有链接到 shared_lib 的目标也有效。
# ${CMAKE_CURRENT_SOURCE_DIR} 指向当前 CMakeLists.txt 文件所在的目录，
# 也就是 src/shared 目录，这样其他模块可以直接引用这里的头文件。
# 特别地，由于 BaseQueue 是模板类，它的实现完全在头文件中，因此需要确保这些头文件能够被访问到。
target_include_directories(shared_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} # 当前目录包含了共享的头文件，特别是模板类的定义
)