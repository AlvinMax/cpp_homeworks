//
// Created by alvinmax on 21.02.19.
//

#ifndef PERSISTENT_SET_PERSISTENT_SET_H
#define PERSISTENT_SET_PERSISTENT_SET_H

#include "my_shared_ptr.h"
#include "my_linked_ptr.h"
#include <vector>
#include <algorithm>

template<typename T, template<typename> class P = my_shared_ptr>
struct persistent_set {
    struct iterator;
    friend struct iterator;

    persistent_set() noexcept : root(P<base_node>(nullptr), P<base_node>(nullptr)) {}

    persistent_set(persistent_set const &other) noexcept : root(other.root) {}

    persistent_set(persistent_set &&other) noexcept : root(std::move(other.root)) {}

    void swap(persistent_set &other) noexcept {
        root.left.swap(other.root.left);
    }

    persistent_set &operator=(persistent_set const &other) noexcept {
        persistent_set temp(other);
        swap(temp);
        return *this;
    }

    persistent_set &operator=(persistent_set &&other) noexcept {
        swap(other);
        return *this;
    }

    iterator find(const T &key) noexcept;

    std::pair<iterator, bool> insert(T key);

    void erase(iterator const &position);

    iterator begin() const noexcept {
        base_node const *temp = &root;
        while (temp->left) {
            temp = temp->left.get();
        }
        return iterator(&root, temp);
    };

    iterator end() const noexcept {
        return iterator(&root, &root);
    };

    bool empty() const noexcept {
        return begin() == end();
    };

    ~persistent_set() = default;

private:
    struct base_node {
        friend struct persistent_set;

        base_node(P<base_node> left_, P<base_node> right_) noexcept : left(std::move(left_)),
                                                                      right(std::move(right_)) {}

        base_node(base_node const &other) noexcept : left(other.left), right(other.right) {}

        base_node(base_node &&other) noexcept : left(std::move(other.left)), right(std::move(other.right)) {}

        void swap(base_node &other) noexcept {
            left.swap(other.left);
            right.swap(other.right);
        }

        base_node &operator=(base_node const &other) noexcept {
            base_node temp(other);
            swap(temp);
            return *this;
        }

        base_node &operator=(base_node &&other) noexcept {
            swap(other);
            return *this;
        }

        friend void swap(base_node &left, base_node &right) noexcept;

    private:
        P<base_node> left, right;
    };

    struct node : base_node {
        friend struct persistent_set;

        explicit node(T data_) noexcept : base_node(P<base_node>(nullptr), P<base_node>(nullptr)),
                                          data(std::move(data_)) {}

        node(node const &other) noexcept : base_node(other), data(other.data) {}

        node(node &&other) noexcept : base_node(std::move(other)), data(std::move(other.data)) {}

        node(P<base_node> left_, P<base_node> right_, T data_) noexcept : base_node(std::move(left_),
                                                                                    std::move(right_)),
                                                                          data(std::move(data_)) {}

        void swap(node &other) noexcept {
            base_node::swap(other);
            std::swap(data, other.data);
        }

        node &operator=(node const &other) noexcept {
            node temp(other);
            swap(temp);
            return *this;
        }

        node &operator=(node &&other) noexcept {
            swap(other);
            return *this;
        }

        friend void swap(node &left, node &right) noexcept;

    private:
        T data;
    };

    base_node *copy_path(std::vector<base_node *> const &path) {
        base_node *v = path[0];
        for (int i = 1; i < path.size(); ++i) {
            auto current = static_cast<node const *>(path[i])->data;
            if (static_cast<node const *>(v)->data < current) {
                v = new node(P<base_node>(v), path[i]->right, current);
            } else {
                v = new node(path[i]->left, P<base_node>(v), current);
            }
        }
        return v;
    }

    P<base_node> recursive_erase(base_node *root, T const &x) {
        auto current = static_cast<node const *>(root)->data; 
        if (x < current) {
            return P<base_node>(new node(
                    recursive_erase(root->left.get(), x),
                    root->right,
                    current
            ));
        } else if (current < x) {
            return P<base_node>(new node(
                    root->left,
                    recursive_erase(root->right.get(), x),
                    current
            ));
        } else {
            if (!root->left && !root->right) {
                return P<base_node>(nullptr);
            }
            if (root->left && !root->right) {
                return root->left;
            }
            if (!root->left && root->right) {
                return root->right;
            }
            if (root->left && root->right) {
                std::vector<base_node *> path;
                base_node *v = root->right.get();
                while (v != nullptr) {
                    path.push_back(v);
                    v = v->left.get();
                }

                auto next_value = static_cast<node const *>(path[path.size() - 1])->data;

                if (path.size() == 1) {
                    return P<base_node>(new node(
                            root->left,
                            path[path.size() - 1]->right,
                            next_value
                    ));
                } else {
                    base_node *with_erased_next = new node(path[path.size() - 1]->right, path[path.size() - 2]->right,
                                                           static_cast<node const *>(path[path.size() - 2])->data);
                    path.pop_back();
                    path.pop_back();
                    path.push_back(with_erased_next);
                    std::reverse(path.begin(), path.end());

                    base_node *new_right = copy_path(path);
                    return P<base_node>(new node(root->left, P<base_node>(new_right), next_value));
                }
            }
        }
    }

    base_node root;
};


template<typename T, template<typename> class P>
struct persistent_set<T, P>::iterator {
    friend struct persistent_set<T, P>;

    iterator() noexcept : root(nullptr), ptr(nullptr) {}

    iterator(iterator const &) = default;

    T const &operator*() const noexcept {
        return (static_cast<node const *>(ptr))->data;
    };

    iterator &operator++() {
        if (root != ptr) {
            next(root->left.get(), root, (static_cast<node const *>(ptr))->data);
            return *this;
        }
    }

    iterator operator++(int postfix) {
        iterator temp(*this);
        ++(*this);
        return temp;
    }

    iterator &operator--() {
        if (root == ptr) {
            if (root->left.get() != nullptr) {
                ptr = rightest(root->left.get());
            }
        } else {
            prev(root->left.get(), root, static_cast<node const *>(ptr)->data);
        }
        return *this;
    };

    iterator operator--(int postfix) {
        iterator temp(*this);
        --(*this);
        return temp;
    }

    bool operator==(iterator const &other) const noexcept {
        return root == other.root && ptr == other.ptr;
    }

    bool operator!=(iterator const &other) const noexcept {
        return !(*this == other);
    }


private:
    iterator(base_node const *root_, base_node const *ptr_) noexcept : root(root_), ptr(ptr_) {}

    base_node const *leftest(base_node *v) const noexcept {
        if (v->left.get() == nullptr) return v;
        return leftest(v->left.get());
    }

    base_node const *rightest(base_node *v) const noexcept {
        if (v->right.get() == nullptr) return v;
        return rightest(v->right.get());
    }

    bool next(base_node const *v, base_node const *ancestor, T const &value) noexcept {
        bool found = false;
        auto current = static_cast<node const *>(v)->data;

        if (value < current) {
            found = next(v->left.get(), v, value);
        }
        if (current < value) {
            found = next(v->right.get(), v, value);
        }
        if (value == current && v->right) {
            ptr = leftest(v->right.get());
            found = true;
        }
        if (!found && ancestor->left.get() == v) {
            ptr = ancestor;
            found = true;
        }

        return found;
    };

    bool prev(base_node const *v, base_node const *ancestor, T const &value) noexcept {
        bool found = false;
        auto current = static_cast<node const *>(v)->data;

        if (value < current) {
            found = prev(v->left.get(), v, value);
        }
        if (current < value) {
            found = prev(v->right.get(), v, value);
        }
        if (value == current && v->left) {
            ptr = rightest(v->left.get());
            found = true;
        }
        if (!found && ancestor->right.get() == v) {
            ptr = ancestor;
            found = true;
        }

        return found;
    }

    base_node const *root;
    base_node const *ptr;
};

template<typename T, template<typename> typename P>
void swap(typename persistent_set<T, P>::base_node &a, typename persistent_set<T, P>::base_node &b) noexcept {
    a.swap(b);
}

template<typename T, template<typename> typename P>
void swap(typename persistent_set<T, P>::node &a, typename persistent_set<T, P>::node &b) noexcept {
    a.swap(b);
}

template<typename T, template<typename> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::find(const T &x) noexcept {
    if (empty()) {
        return end();
    }

    base_node const *v = root.left.get();
    while (v != nullptr && !(static_cast<node const *>(v)->data == x)) {
        if (x < static_cast<node const *>(v)->data) {
            v = v->left.get();
        } else {
            v = v->right.get();
        }
    }

    if (v != nullptr) {
        return iterator(&root, v);
    } else {
        return end();
    }
}

template<typename T, template<typename> class P>
std::pair<typename persistent_set<T, P>::iterator, bool> persistent_set<T, P>::insert(T x) {
    iterator it = find(x);
    if (it != end()) {
        return {it, false};
    }

    std::vector<base_node *> path;
    base_node *v = root.left.get();

    while (v != nullptr) {
        path.push_back(v);
        if (x < static_cast<node const *>(v)->data) {
            v = v->left.get();
        } else {
            v = v->right.get();
        }
    }

    base_node *new_node = new node(std::move(x));
    it.ptr = new_node;

    path.push_back(new_node);
    std::reverse(path.begin(), path.end());

    base_node *new_root = copy_path(path);
    root.left = P<base_node>(new_root);

    return {it, true};
}

template<typename T, template<typename> class P>
void persistent_set<T, P>::erase(const persistent_set::iterator &position) {
    if (position == end()) {
        return;
    } else {
        root.left = recursive_erase(root.left.get(), *position);
    }
}


#endif //PERSISTENT_SET_PERSISTENT_SET_H
