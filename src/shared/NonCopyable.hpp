#pragma once

/**
 * @brief 防止拷贝构造和赋值操作的基类。
 *
 * NonCopyable 类用于防止继承自它的类被复制或赋值。通过将拷贝构造函数和赋值操作符声明为删除，
 * 可以确保任何尝试进行复制或赋值的操作都会导致编译错误，从而避免潜在的资源管理问题。
 *
 * 使用示例：
 * @code
 * class MyClass : private NonCopyable {
 *     // MyClass 不能被复制或赋值
 * };
 * @endcode
 */
class NonCopyable {
protected:
    /**
     * @brief 默认构造函数。
     *
     * 将构造函数设为私有，防止外部实例化，但允许派生类构造。
     */
    NonCopyable() = default;

    /**
     * @brief 拷贝构造函数（已删除）。
     *
     * 禁止拷贝构造，防止对象被复制。
     */
    NonCopyable(const NonCopyable&) = delete;

    /**
     * @brief 赋值操作符（已删除）。
     *
     * 禁止赋值操作，防止对象被赋值。
     *
     * @return NonCopyable& 返回引用自身（未定义行为，因为该函数已被删除）。
     */
    NonCopyable& operator=(const NonCopyable&) = delete;
};