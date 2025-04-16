#pragma once

#include <memory>
#include "../mutex/MutexLock.hpp"
#include <stdexcept>

template<typename T>
class BaseQueue {
protected:
    /**
     * @brief 双向链表队列基础类中的节点信息
     */
    struct Node {
        /**
         * @brief 数据存储字段。
         */
        T data_;

        /**
         * @brief 使用 std::unique_ptr<Node> 表示对下一个节点的独占所有权。
         *
         * 当一个节点持有另一个节点的 std::unique_ptr 时，它对该下一个节点具有唯一的控制权。
         * 这意味着：
         * - 如果节点 A 的 next_ 指向节点 B，则节点 A 对节点 B 有唯一的控制权。
         * - 当节点 A 被删除时，它所拥有的 next_（即节点 B）也会自动被删除。
         * 这种设置有助于确保内存安全，避免内存泄漏，并简化内存管理。
         */
        std::unique_ptr<Node> next_;

        /**
         * @brief 指向前一个节点的普通指针。
         *
         * 由于双向链表的特性，prev_ 并不拥有前一个节点的所有权，而只是简单地指向它。
         * 这允许在不影响所有权关系的情况下实现双向遍历。
         */
        Node* prev_;

        Node(const T& val) : data_(val), next_(nullptr), prev_(nullptr) {}
    };

    /**
     * @brief 使用 std::unique_ptr 管理链表的头部节点，确保自动释放资源。
     *
     * - 当队列为空时，head_ 为 nullptr。
     * - 在插入新元素到队头或删除队头元素时，会相应地更新 head_。
     * - 使用 std::unique_ptr 提供独占所有权，简化内存管理并防止内存泄漏。
     */
    std::unique_ptr<Node> head_;

    /**
     * @brief 指向链表尾部节点的普通指针。
     *
     * - tail_ 不拥有对尾部节点的所有权，仅用于快速访问和更新链表的尾部。
     * - 在插入新元素到队尾时，tail_ 会被更新以指向新的尾部节点。
     * - 如果链表为空，则 tail_ 为 nullptr。
     */
    Node* tail_;

    /**
     * @brief 记录链表中元素的数量。
     *
     * - 每次通过 push_back 或 push_front 方法添加元素时，count_ 增加。
     * - 每次通过 pop_back 或 pop_front 方法移除元素时，count_ 减少。
     * - 可用于快速查询链表的大小，而无需遍历整个链表。
     */
    size_t count_;

    /**
     * @brief 互斥锁，用于保护共享资源免受并发访问的影响。
     *
     * - 在多线程环境中，使用 MutexLock 来确保对链表的任何修改（如插入或删除元素）都是线程安全的。
     * - 在执行可能改变链表状态的操作之前，应首先锁定互斥锁，并在操作完成后解锁。
     * - 通常与 MutexLockGuard 一起使用，以确保即使在发生异常的情况下也能正确释放锁。
     */
    MutexLock mutex_;

    /**
     * @brief BaseQueue 类的默认构造函数。
     *
     * 初始化以下成员变量：
     * - head_ 设置为 nullptr，表示链表初始为空。
     * - tail_ 设置为 nullptr，表示链表初始为空。
     * - count_ 设置为 0，表示链表初始包含 0 个元素。
     * - mutex_ 使用其默认构造函数进行初始化，准备用于后续的线程同步操作。
     */
    BaseQueue() : head_(nullptr), tail_(nullptr), count_(0) {}

public:
    /**
     * @brief 虚析构函数，确保派生类能够正确析构。
     *
     * 将析构函数声明为虚函数，使得当通过基类指针或引用删除派生类对象时，
     * 可以调用派生类的析构函数，从而避免资源泄漏和其他潜在问题。
     * 使用 `= default` 表示使用编译器生成的默认析构函数实现。
     */
    virtual ~BaseQueue() = default;

    /**
     * @brief 在队列尾部插入一个新元素。
     *
     * - 将给定值添加到队列的尾部。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @param value 要插入的新元素。
     */
    void push_back(const T& value);

    /**
     * @brief 在队列头部插入一个新元素。
     *
     * - 将给定值添加到队列的头部。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @param value 要插入的新元素。
     */
    void push_front(const T& value);

    /**
     * @brief 移除并返回队列尾部的元素。
     *
     * - 如果队列为空，则抛出 std::underflow_error 异常。
     * - 返回被移除的元素。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return T 队列尾部的元素。
     */
    T pop_back();

    /**
     * @brief 移除并返回队列头部的元素。
     *
     * - 如果队列为空，则抛出 std::underflow_error 异常。
     * - 返回被移除的元素。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return T 队列头部的元素。
     */
    T pop_front();

    /**
     * @brief 从队列中移除指定的节点。
     *
     * - 如果节点不存在于队列中，则抛出 std::invalid_argument 异常。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @param node 要移除的节点指针。
     *
     * @return T 删除的元素。
     */
    T remove(Node* node);

    /**
     * @brief 检查队列是否为空。
     *
     * - 如果队列为空，则返回 true；否则返回 false。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return bool 队列是否为空。
     */
    bool empty() const { return count_ == 0; }

    /**
     * @brief 返回队列中元素的数量。
     *
     * - 返回当前队列中的元素数量。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return size_t 队列中的元素数量。
     */
    size_t size() const { return count_; }

    /**
     * @brief 返回队列头部元素的引用。
     *
     * - 如果队列为空，则抛出 std::underflow_error 异常。
     * - 否则，返回队列头部元素的引用。
     * - 注意：此方法假定队列非空，因此在调用前应确保队列不为空。
     *
     * @return T& 队列头部元素的引用。
     */
    T& front();

    /**
     * @brief 返回队列尾部元素的引用。
     *
     * - 如果队列为空，则抛出 std::underflow_error 异常。
     * - 否则，返回队列尾部元素的引用。
     * - 注意：此方法假定队列非空，因此在调用前应确保队列不为空。
     *
     * @return T& 队列尾部元素的引用。
     */
    T& back();

    class iterator {
    private:
        Node* current;
    public:
        /**
         * @brief 构造函数，初始化迭代器指向指定节点。
         *
         * @param node 初始化迭代器指向的节点。
         */
        iterator(Node* node) : current(node) {}

        /**
         * @brief 解引用操作符，返回当前节点的数据。
         *
         * @return T& 当前节点的数据。
         */
        T& operator*() const { return current->data_; }

        /**
         * @brief 成员访问操作符，返回一个指向当前节点数据的指针。
         *
         * @return T* 指向当前节点数据的指针。
         */
        T* operator->() const { return &(current->data_); }

        /**
        * @brief 前置递增操作符，将迭代器移动到下一个节点。
        *
        * @return iterator& 修改后的迭代器。
        */
        iterator& operator++() { current = current->next_.get(); return *this; }

        /**
        * @brief 后置递增操作符，将迭代器移动到下一个节点，并返回原迭代器。
        *
        * @return iterator 原迭代器副本。
        */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief 判断两个迭代器是否相等。
         *
         * @param other 要比较的另一个迭代器。
         * @return bool 是否相等。
         */
        bool operator==(const iterator& other) const { return current == other.current; }

        /**
         * @brief 判断两个迭代器是否不相等。
         *
         * @param other 要比较的另一个迭代器。
         * @return bool 是否不相等。
         */
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };

    /**
     * @brief 返回指向队列头部的迭代器。
     *
     * - 如果队列为空，迭代器将指向 nullptr。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return iterator 指向队列头部的迭代器。
     */
    iterator begin() {
        MutexLockGuard autoLock(mutex_);
        return iterator(head_.get());
    }

    /**
     * @brief 返回指向队列末尾的迭代器（即 end 迭代器）。
     *
     * - 返回一个指向 nullptr 的迭代器，表示队列的结束。
     * - 线程安全：该操作在执行时会加锁以防止并发访问问题。
     *
     * @return iterator 指向队列末尾的迭代器。
     */
    iterator end() {
        MutexLockGuard autoLock(mutex_);
        return iterator(nullptr);
    }
};

template<typename T>
void BaseQueue<T>::push_back(const T& value) {
    MutexLockGuard autoLock(mutex_);
    auto newNode = std::make_unique<Node>(value);
    if (empty()) {
        head_ = std::move(newNode);
        tail_ = head_.get();
    } else {
        tail_->next_ = std::move(newNode);
        tail_->next_->prev_ = tail_;
        tail_ = tail_->next_.get();
    }
    ++count_;
}

template<typename T>
void BaseQueue<T>::push_front(const T& value) {
    MutexLockGuard autoLock(mutex_);
    auto newNode = std::make_unique<Node>(value);
    if (empty()) {
        head_ = std::move(newNode);
        tail_ = head_.get();
    } else {
        // 将当前 head_ 的所有权转移给 newNode->next_
        // 底层机制：
        // 1. std::move(head_) 将 head_ 转换为右值引用，允许其资源被“窃取”。
        //    - 此操作不会改变 head_ 的内容本身，而是使其成为临时对象，允许后续的移动操作。
        // 2. 执行移动赋值操作 newNode->next_ = std::move(head_);
        //    - 移动赋值操作符从 head_ “窃取”其内部指针（即指向节点 A 的指针），并将这个指针赋值给 newNode->next_。
        //    - 在此过程中，head_ 内部的指针被置为 nullptr，因为它已经失去了对节点 A 的所有权。
        // 结果：
        // - newNode->next_ 现在指向原来的头节点（节点 A）。
        // - head_ 已经失去对原头节点的所有权，其内部指针现在为 nullptr。
        // 注意：
        // - 接下来，通过 newNode->next_.get() 可以访问原来的头节点 A。
        // - 必须更新原头节点的 prev_ 指针，使其指向新节点 B，即 newNode->next_->prev_ = newNode.get();
        newNode->next_ = std::move(head_);
        newNode->next_->prev_ = newNode.get();
        head_ = std::move(newNode);
    }
    ++count_;
}

template<typename T>
T BaseQueue<T>::pop_back() {
    MutexLockGuard autoLock(mutex_);

    if (empty()) {
        throw std::underflow_error("List is empty");
    }

    if (head_->next_ == nullptr) { // 只有一个元素
        T value = std::move(head_->data_);
        head_.reset();
        tail_ = nullptr;
        --count_;
        return value;
    } else {
        // 更新 tail_ 指向倒数第二个节点
        Node* old_tail = tail_;
        tail_ = tail_->prev_;
        T value = std::move(old_tail->data_);
        tail_->next_.reset(); // 释放旧尾节点
        --count_;
        return value;
    }
}

template<typename T>
T BaseQueue<T>::pop_front() {
    MutexLockGuard autoLock(mutex_);

    if (empty()) {
        throw std::underflow_error("List is empty");
    }

    // 使用 std::move 将 head_->data_ 的资源移动到 value 中。
    // 通过这种方式，避免了不必要的深拷贝，提高了性能。
    // 注意：这行代码假设 T 类型支持移动语义（即定义了移动构造函数）。
    // 如果 T 不支持移动语义，则会退回到拷贝构造函数。
    // 移动后，head_->data_ 将处于一个有效的但未定义的状态（例如，其内部资源可能被置为默认值或 nullptr）。
    // 在本例中，head_->data_ 的资源被转移到 value，而 head_->data_ 本身不再拥有这些资源。
    T value = std::move(head_->data_);

    if (head_->next_ == nullptr) { // 只有一个元素
        head_.reset();
        tail_ = nullptr;
    } else {
        // 移动 head_ 到下一个节点，并更新 prev_ 指针。
        // 由于之前节点的 head_->data_ 的资源已经转移，此时对 head_ 重新赋值，head_->data_ 会被释放。
        // 但因为 head_->data_ 的资源已经转移，因此不会影响对 T 的获取。
        head_ = std::move(head_->next_);
        head_->prev_ = nullptr;
    }
    --count_;
    return value;
}

template<typename T>
T BaseQueue<T>::remove(Node* node) {
    if (!node || empty()) {
        throw std::invalid_argument("Invalid node or queue is empty");
    }

    // 检查节点是否是头节点
    if (node == head_.get()) {
        return pop_front();
    }

    // 检查节点是否是尾节点
    if (node == tail_) {
        return pop_back();
    }

    MutexLockGuard autoLock(mutex_);

    T value = std::move(node->data_);

    // 节点是中间节点
    Node* prevNode = node->prev_;
    Node* nextNode = node->next_.get();

    if (!prevNode || !nextNode) {
        throw std::invalid_argument("Node is not part of the queue");
    }

    // 更新前驱节点和后继节点之间的链接
    prevNode->next_ = std::move(node->next_);
    nextNode->prev_ = prevNode;

    // 减少计数
    --count_;

    return value;
}

template<typename T>
T& BaseQueue<T>::front() {
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data_;
}

template<typename T>
T& BaseQueue<T>::back() {
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data_;
}