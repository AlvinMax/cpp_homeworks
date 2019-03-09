//
// Created by alvinmax on 21.02.19.
//

#ifndef PERSISTENT_SET_MY_LINKED_PTR_H
#define PERSISTENT_SET_MY_LINKED_PTR_H
#include <iostream>
#include <cstdio>

template <typename T>
struct my_linked_ptr {
    my_linked_ptr() noexcept : ptr(nullptr), prev(nullptr), next(nullptr) {}
    explicit my_linked_ptr(T* ptr_) noexcept : ptr(ptr_), prev(nullptr), next(nullptr) {}

    my_linked_ptr(my_linked_ptr const &other) noexcept : ptr(other.ptr), prev(const_cast<my_linked_ptr*>(&other)), next(other.next) {
        init();
    }

    my_linked_ptr(my_linked_ptr && other) noexcept : ptr(other.ptr), prev(other.prev), next(other.next) {
        other.ptr = nullptr;
        other.prev = nullptr;
        other.next = nullptr;
        init();
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    void swap(my_linked_ptr &other) noexcept {
        if (ptr == other.ptr) return;
        std::swap(ptr, other.ptr);
        std::swap(prev, other.prev);
        std::swap(next, other.next);
        init();
        other.init();
    }

    my_linked_ptr& operator=(my_linked_ptr const& other) noexcept {
        my_linked_ptr temp(other);
        swap(temp);
        return *this;
    }

    my_linked_ptr& operator=(my_linked_ptr && other) noexcept {
        swap(other);
        return *this;
    }

    ~my_linked_ptr() {
        if (ptr != nullptr) {
            if (prev == nullptr && next == nullptr) {
                delete ptr;
            } else if (prev == nullptr && next != nullptr) {
                next->prev = nullptr;
            } else if (prev != nullptr && next == nullptr) {
                prev->next = nullptr;
            } else {
                prev->next = next;
                next->prev = prev;
            }
        }
    }

    friend void swap(my_linked_ptr &left, my_linked_ptr &right) noexcept;

private:
    T* ptr;
    my_linked_ptr *prev;
    my_linked_ptr *next;

    void init() noexcept {
        if (ptr == nullptr) return;
        if (prev != nullptr) {
            prev->next = this;
        }
        if (next != nullptr) {
            next->prev = this;
        }
    }
};

template <typename T>
void swap(my_linked_ptr<T> &left, my_linked_ptr<T> &right) noexcept {
    left.swap(right);
}

#endif //PERSISTENT_SET_MY_LINKED_PTR_H
