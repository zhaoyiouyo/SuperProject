#pragma once

/**
 * @brief 定义了磁带驱动器可能执行的各种操作。
 *
 * 每个枚举值对应一种特定的操作类型：
 * - READ_AGGR: 读聚合操作
 * - WRITE_AGGR: 写聚合操作
 * - LOAD_TAPE: 加载磁带操作
 * - UNLOAD_TAPE: 卸载磁带操作
 * - INVENTORY: 库存检查操作
 * - ROLL_TAPE: 卷带操作
 */
enum class TypeOperation {
    READ_AGGR = 1,    ///< 读聚合操作
    WRITE_AGGR,       ///< 写聚合操作
    LOAD_TAPE,        ///< 加载磁带操作
    UNLOAD_TAPE,      ///< 卸载磁带操作
    INVENTORY,        ///< 库存检查操作
    ROLL_TAPE         ///< 卷带操作
};

class TapeDrivesOperation {
public:
    /**
     * @brief 构造函数，初始化 TapeDrivesOperation 对象。
     *
     * @param op 初始的操作类型。
     */
    TapeDrivesOperation(TypeOperation op);

    /**
     * @brief 获取当前的操作类型。
     *
     * @return TypeOperation 当前的操作类型。
     */
    TypeOperation getTypeOperation();

    /**
     * @brief 设置新的操作类型。
     *
     * @param op 新的操作类型。
     */
    void setTypeOperation(TypeOperation op);


private:
    TypeOperation op_; ///< 存储当前的操作类型。
};