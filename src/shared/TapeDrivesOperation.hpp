#pragma once

enum class TypeOperation {
    READ_AGGR = 1,
    WRITE_AGGR,
    LOAD_TAPE,
    UNLOAD_TAPE,
    INVENTORY,
    ROLL_TAPE
};

class TapeDrivesOperation {
public:
    TapeDrivesOperation(TypeOperation op);

    /**
     * @param 获取操作类型
     * @return 枚举类型，磁带的操作
     */
    TypeOperation getTypeOperation();

    /**
     * @param 修改操作
     * @param op 枚举类型，磁带的操作
     */
    void setTypeOperation(TypeOperation op);


private:
    TypeOperation op_;
};