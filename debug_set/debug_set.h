//
// Created by Maxim on 15.03.2018.
//

#ifndef DEBUG_SET_DEBUG_SET_H
#define DEBUG_SET_DEBUG_SET_H

#include <iterator>
#include <cassert>

template<typename T>
class debug_set {
private:

    class base_iterator {
    public:
        virtual void invalid() = 0;

        virtual void update_owner(debug_set<T> const *) = 0;

        virtual base_iterator *&get_next() = 0;
    };

    template<typename VT>
    class my_iterator;

    class node {
    public:
        T data;
        base_iterator *begin_iterator, *end_iterator;
        node *prev, *left, *right;

        node() : data(0), prev(nullptr), left(nullptr), right(nullptr), begin_iterator(nullptr), end_iterator(nullptr) {}

        node(T data) : data(data), prev(nullptr), left(nullptr), right(nullptr), begin_iterator(nullptr),
                       end_iterator(nullptr) {}

        static void add_left(node *x, node *y) {
            x->left = y;
            y->prev = x;
        }

        static void add_right(node *x, node *y) {
            x->right = y;
            y->prev = x;
        }

        void insert(base_iterator *other) {
            if (begin_iterator == nullptr) {
                begin_iterator = end_iterator = other;
            } else {
                end_iterator->get_next() = other;
                end_iterator = other;
            }
        }

        void erase(base_iterator *other) {
            if (other == begin_iterator) {
                begin_iterator = begin_iterator->get_next();
            } else {
                for (base_iterator *prev_i = begin_iterator, *i = begin_iterator->get_next();
                     i != nullptr; prev_i = i, i = i->get_next()) {
                    if (i == other) {
                        if (end_iterator == i) {
                            end_iterator = prev_i;
                        }
                        prev_i->get_next() = i->get_next();
                        break;
                    }
                }
            }
            other->get_next() = nullptr;
        }

        void update_owner_iterators(debug_set<T> const *new_owner) {
            for (base_iterator *i = begin_iterator; i != nullptr; i = i->get_next()) {
                i->update_owner(new_owner);
            }
        }

        void invalid_all_iterators() {
            for (base_iterator *i = begin_iterator; i != nullptr; i = i->get_next()) {
                i->invalid();
            }
        }

        ~node() {
            invalid_all_iterators();
        }
    };

    node *begin_node, *end_node, *root;

    void make_changeable() {
        begin_node->prev->left = nullptr;
        end_node->prev->right = nullptr;
        begin_node->prev = nullptr;
        end_node->prev = nullptr;
    }

    void set_iterators() {
        if (root == nullptr) {
            node::add_right(begin_node, end_node);
            node::add_left(end_node, begin_node);
            return;
        }

        begin_node->left =
        begin_node->right =
        end_node->left =
        end_node->right = nullptr;

        node *i = root;
        while ((i->left) != nullptr) {
            i = i->left;
        }
        node::add_left(i, begin_node);

        node *j = root;
        while ((j->right) != nullptr) {
            j = j->right;
        }
        node::add_right(j, end_node);
    }

public:
    typedef my_iterator<T> iterator;
    typedef my_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    debug_set() : begin_node(), end_node(), root() {
        begin_node = new node();
        try {
            end_node = new node();
        } catch (...) {
            delete begin_node;
        }
        root = nullptr;
        set_iterators();
    }

    debug_set(debug_set const &other) : debug_set() {
        for (node *i = other.begin_node->prev; i != other.end_node; i = i->prev) {
            insert(i->data);
        }
    }

    bool empty() const {
        return root == nullptr;
    }

    void clear() {
        while (!empty()) {
            debug_set::const_iterator it = begin();
            erase(it);
        }
    }

    void put_by_other(node *v, debug_set &ds) {
        if (v == nullptr || v == end_node || v == begin_node) {
            return;
        }
        ds.insert(v->data);
        put_by_other(v->left, ds);
        put_by_other(v->right, ds);
    }

    debug_set &operator=(debug_set const &other) {
        debug_set temp;
        put_by_other(other.root, temp);
        clear();
        put_by_other(temp.root, *this);
        return *this;
    }

    iterator begin() {
        return iterator(begin_node->prev, this);
    }

    iterator end() {
        return iterator(end_node, this);
    }
    const_iterator begin() const {
        return const_iterator(begin_node->prev, this);
    }

    const_iterator end() const {
        return const_iterator(end_node, this);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    std::pair<iterator, bool> insert(T const &new_data) {
        node *temp = new node(new_data);
        if (root == nullptr) {
            root = temp;
            set_iterators();
            return {iterator(root, this), true};
        }

        make_changeable();

        node *next_node = root, *pre_node = nullptr;


        while (next_node != nullptr) {
            pre_node = next_node;
            if (next_node->data > temp->data) {
                next_node = next_node->left;
            } else {
                next_node = next_node->right;
            }
        }

        if (pre_node->data == temp->data) {
            set_iterators();
            return {iterator(pre_node, this), false};
        }

        if (pre_node->data > temp->data) {
            node::add_left(pre_node, temp);
        } else {
            node::add_right(pre_node, temp);
        }

        set_iterators();
        return {iterator(temp, this), true};

    }

    const_iterator erase(const_iterator const &it) {
        assert(this == it.owner);
        assert(it._node != end_node);

        make_changeable();
        node *replacement = nullptr, *replaceParent = nullptr;

        node *node_ = it._node;
        node *temp = node_;

        if (temp->left == nullptr && temp->right == nullptr) {
            replacement = temp;
            replaceParent = temp->prev;

            if (replacement != root) {
                if (temp->prev->left == temp) {
                    temp->prev->left = nullptr;
                } else {
                    temp->prev->right = nullptr;
                }
            } else root = nullptr;
        } else {
            if (temp->left != nullptr) {
                temp = temp->left;
                while (temp != nullptr) {
                    replacement = temp;
                    temp = temp->right;
                }

                replaceParent = replacement->prev;

                if (replacement->prev->left == replacement)
                    replacement->prev->left = replacement->left;
                else
                    replacement->prev->right = replacement->left;

                if (replacement->left != nullptr)
                    replacement->left->prev = replacement->prev;
            } else {
                replacement = temp;
                replaceParent = replacement->prev;

                if (replacement == root) {
                    replacement->right->prev = nullptr;
                    root = replacement->right;
                } else {
                    if (replacement->prev->left == replacement)
                        replacement->prev->left = replacement->right;
                    else
                        replacement->prev->right = replacement->right;

                    if (replacement->right != nullptr)
                        replacement->right->prev = replacement->prev;
                }
            }
        }

        if (replacement != node_) node_->data = replacement->data;

        delete replacement;
        set_iterators();

        return const_iterator(replaceParent, this);
    }


    friend void update(node *v, debug_set &new_owner) {
        if (v == nullptr) return;
        v->update_owner_iterators(&new_owner);
        update(v->left, new_owner);
        update(v->right, new_owner);
    }

    friend void swap(debug_set &a, debug_set &b) {
        a.make_changeable();
        b.make_changeable();

        update(a.root, b);
        update(b.root, a);

        std::swap(a.begin_node, b.begin_node);
        std::swap(a.end_node, b.end_node);
        std::swap(a.root, b.root);

        a.set_iterators();
        b.set_iterators();
    }

    const_iterator lower_bound(const T &value) const {
        node *next_node = root, *pre_node = nullptr;

        while (next_node != nullptr) {
            pre_node = next_node;
            if (next_node->data == value) break;
            if (next_node->data > value) {
                next_node = next_node->left;
            } else {
                next_node = next_node->right;
            }
        }
        return iterator(pre_node, this);
    }

    const_iterator upper_bound(const T &value) const {
        node *next_node = root, *pre_node = nullptr;

        while (next_node != nullptr) {
            pre_node = next_node;
            if (next_node->data > value) {
                next_node = next_node->left;
            } else {
                next_node = next_node->right;
            }
        }
        return iterator(pre_node, this);
    }

    const_iterator find(const T &element) const {
        const_iterator result = lower_bound(element);
        if (result._node != end_node && *result == element) return result;
        else return const_iterator(end_node, this);
    }

    ~debug_set() {
        clear();
        delete begin_node;
        delete end_node;
        if (root != nullptr) delete root;
    }
};

template<typename T>
template<typename VT>
class debug_set<T>::my_iterator : public debug_set<T>::base_iterator {
private:
    friend debug_set<T>;
    friend debug_set<T>::node;
    node *_node;
    bool is_invalid;
    debug_set<T> const *owner;
    typename debug_set<T>::base_iterator *next;

    void invalid() override {
        assert(!is_invalid);
        is_invalid = true;
    }

    void update_owner(debug_set<T> const *new_owner) override {
        owner = new_owner;
    }

    typename debug_set<T>::base_iterator *&get_next() override {
        return next;
    }

public:

    my_iterator() : _node(nullptr), is_invalid(true), next(nullptr) {}

    my_iterator(my_iterator const &other) :
            _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(!other.is_invalid);
        _node->insert(this);
    }

    my_iterator(node *_node, debug_set<T> const *_owner)
            : _node(_node), is_invalid(false) {
        owner = _owner;
        next = nullptr;
        if (_node != nullptr) _node->insert(this);
    }

    template<typename OTHER_TYPE>
    my_iterator(const my_iterator <OTHER_TYPE> &other,
                typename std::enable_if<std::is_same<typename std::remove_const<VT>::type, OTHER_TYPE>::value>::type * = nullptr)
            : _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(other.is_invalid == false);
        _node->insert(this);
    }

    my_iterator &operator=(my_iterator const &other) {
        assert(!other.is_invalid);
        if (!is_invalid) {
            _node->erase(this);
        }
        _node = other._node;
        is_invalid = other.is_invalid;
        owner = other.owner;
        _node->insert(this);
        return *this;
    }

    VT &operator*() const {
        assert(!is_invalid);
        assert(_node != owner->end_node);
        assert(_node != owner->begin_node);
        return _node->data;
    }

    my_iterator &operator++() {
        assert(!is_invalid);
        assert(_node != owner->end_node);
        _node->erase(this);

        if (_node->right != nullptr) {
            _node = _node->right;
            while ((_node->left) != nullptr) {
                _node = _node->left;
            }
        } else {
            while (_node->prev->left != _node) {
                _node = _node->prev;
            }
            _node = _node->prev;
        }

        _node->insert(this);
        return *this;
    }

    my_iterator operator++(int) {
        assert(!is_invalid);
        assert(_node != owner->end_node);
        my_iterator temp(*this);
        ++(*this);
        return temp;
    }

    my_iterator &operator--() {
        assert(!is_invalid);
        assert(_node != owner->begin_node);
        _node->erase(this);

        if (_node->left != nullptr) {
            _node = _node->left;
            while ((_node->right) != nullptr) {
                _node = _node->right;
            }
        } else {
            while (_node->prev->right != _node) {
                _node = _node->prev;
            }
            _node = _node->prev;
        }

        _node->insert(this);
        return *this;
    }

    my_iterator operator--(int) {
        assert(!is_invalid);
        assert(_node != owner->begin_node);
        my_iterator temp(*this);
        --(*this);
        return temp;
    }

    ~my_iterator() {
        if (!is_invalid && _node != nullptr) {
            _node->erase(this);
        }
    }

    friend bool operator==(my_iterator const &a, my_iterator const &b) {
        assert(!a.is_invalid);
        assert(!b.is_invalid);
        return a._node == b._node;
    }

    friend bool operator!=(my_iterator const &a, my_iterator const &b) {
        assert(!a.is_invalid);
        assert(!b.is_invalid);
        return a._node != b._node;
    }

    friend void swap(my_iterator &a, my_iterator &b) {
        assert(a.owner == b.owner);
        assert(!a.is_invalid);
        assert(!b.is_invalid);
        a._node->erase(&a);
        b._node->erase(&b);
        std::swap(a._node, b._node);
        a._node->insert(&a);
        b._node->insert(&b);
    }

    typedef std::ptrdiff_t difference_type;
    typedef VT value_type;
    typedef VT *pointer;
    typedef VT &reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};


#endif //DEBUG_SET_DEBUG_SET_H
