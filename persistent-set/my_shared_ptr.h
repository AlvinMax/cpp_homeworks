//
// Created by alvinmax on 21.02.19.
//

#ifndef PERSISTENT_SET_MY_SHARED_PTR_H
#define PERSISTENT_SET_MY_SHARED_PTR_H

#include <iostream>
#include <cstdio>

template <typename T>
struct my_shared_ptr {
    my_shared_ptr() noexcept : ptr(nullptr), sz(nullptr) {}
    explicit my_shared_ptr(T* ptr_) : ptr(ptr_), sz(nullptr) {
        if (ptr != nullptr) {
            sz = new size_t(1);
        }
    }

    my_shared_ptr(my_shared_ptr const &other) noexcept : ptr(other.ptr), sz(other.sz) {
        if (ptr != nullptr) {
            ++(*sz);
        }
    }

    my_shared_ptr(my_shared_ptr && other) noexcept : ptr(other.ptr), sz(other.sz) {
        other.ptr = nullptr;
        other.sz = nullptr;
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    void swap(my_shared_ptr &other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(sz, other.sz);
    }

    my_shared_ptr& operator=(my_shared_ptr const& other) noexcept {
        my_shared_ptr temp(other);
        swap(temp);
        return *this;
    }

    my_shared_ptr& operator=(my_shared_ptr && other) noexcept {
        swap(other);
        return *this;
    }

    ~my_shared_ptr() {
        if (sz != nullptr) {
            --(*sz);
            if (!(*sz)) {
                delete ptr;
                delete sz;
            }
        }
    }

    friend void swap(my_shared_ptr &left, my_shared_ptr &right) noexcept;

private:
    T* ptr;
    size_t* sz;
};

template <typename T>
void swap(my_shared_ptr<T> &left, my_shared_ptr<T> &right) noexcept {
    left.swap(right);
}

#endif //PERSISTENT_SET_MY_SHARED_PTR_H
