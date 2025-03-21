#pragma once

#include <iostream>
#include <memory>
#include <MutexLock.hpp>
#include <stdexcept>

template<typename T>
class BaseQueue {
protected:
    struct Node {
        T data_;
        std::unique_ptr<Node> next_;
        Node* prev_;

        Node(const T& val) : data_(val), prev_(nullptr) {}
    };

    std::unique_ptr<Node> head_;
    Node* tail_;
    size_t count_;
    MutexLock mutex_;

    // 构造函数
    BaseQueue() : tail_(nullptr), count_(0) {}

public:
    // 虚析构函数，确保派生类能够正确析构
    virtual ~BaseQueue() = default;

    // 公共方法
    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    bool empty() const { return count_ == 0; }
    size_t size() const { return count_; }

    T& front();
    T& back();

    class iterator {
    private:
        Node* current;
    public:
        iterator(Node* node) : current(node) {}

        T& operator*() const { return current->data; }
        iterator& operator++() { current = current->next_.get(); return *this; }
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

        // 添加的方法: 获取当前迭代器所指向的Node
        Node* getNode() const { return current; }
    };

    iterator begin() {
        //1、上锁
        MutexLockGuard autoLock(mutex_);
        return iterator(head_.get());
    }

    iterator end() {
        //1、上锁
        MutexLockGuard autoLock(mutex_);
        return iterator(nullptr);
    }

    // 纯虚函数，强制派生类实现
    virtual void specialOperation() = 0;
};

template<typename T>
void BaseQueue<T>::push_back(const T& value) {
    //1、上锁
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
    //1、上锁
    MutexLockGuard autoLock(mutex_);
    auto newNode = std::make_unique<Node>(value);
    if (empty()) {
        head_ = std::move(newNode);
        tail_ = head_.get();
    } else {
        newNode->next_ = std::move(head_);
        head_->prev_ = newNode.get();
        head_ = std::move(newNode);
    }
    ++count_;
}

template<typename T>
void BaseQueue<T>::pop_back() {
    //1、上锁
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    if (head_->next_ == nullptr) { // Only one element
        head_.reset();
        tail_ = nullptr;
    } else {
        tail_ = tail_->prev_;
        tail_->next_.reset();
    }
    --count_;
}

template<typename T>
void BaseQueue<T>::pop_front() {
    //1、上锁
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    head_ = std::move(head_->next_);
    if (head_ != nullptr) {
        head_->prev_ = nullptr;
    } else {
        tail_ = nullptr;
    }
    --count_;
}

template<typename T>
T& BaseQueue<T>::front() {
    //1、上锁
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    return head_->data;
}

template<typename T>
T& BaseQueue<T>::back() {
    //1、上锁
    MutexLockGuard autoLock(mutex_);
    if (empty()) throw std::underflow_error("List is empty");
    return tail_->data;
}