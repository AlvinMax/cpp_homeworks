//
// Created by Maxim on 10.03.2018.
//

#ifndef DEBUG_debug_list_LIST_H
#define DEBUG_debug_list_LIST_H

#include <vector>
#include <cassert>

template <typename T>
class debug_list {

private:

    class any_iterator {
    public:
        virtual void invalid() = 0;
        virtual void update_owner(debug_list<T> const*) = 0;
        virtual any_iterator *& get_next() = 0;
    };

    template <typename VT> class my_iterator;
    class base_node {
    public:
        base_node *prev, *next;
        any_iterator *begin_iterator, *end_iterator;
        base_node() : prev(nullptr), next(nullptr), begin_iterator(nullptr), end_iterator(nullptr) {}

        static void connect(base_node *x, base_node *y) {
            x->prev = y;
            y->next = x;
        }

        void push_back(any_iterator* other) {
            if(begin_iterator == nullptr) {
                begin_iterator = end_iterator = other;
            } else {
                end_iterator->get_next() = other;
                end_iterator = other;
            }
        }

        void erase_by_iterator(any_iterator* other) {
            if(other == begin_iterator) {
                begin_iterator = begin_iterator->get_next();
            } else {
                for(any_iterator *prev_i = begin_iterator, *i = begin_iterator->get_next(); i!= nullptr; prev_i = i, i=i->get_next()) {
                    if(i == other) {
                        if(end_iterator == i) {
                            end_iterator = prev_i;
                        }
                        prev_i->get_next() = i->get_next();
                        break;
                    }
                }
            }
        }

        void update_owner_iterators(debug_list<T> const* new_owner) {
            for(any_iterator* i = begin_iterator; i != nullptr; i=i->get_next()) {
                i->update_owner(new_owner);
            }
        }

        void invalid_all_iterators() {
            for(any_iterator* i = begin_iterator; i != nullptr; i = i->get_next()) {
                i->invalid();
            }
        }

        virtual ~base_node() {
            invalid_all_iterators();
        }
    };

    class node: public base_node {
    public:
        T data;

        node() = delete;
        node(T data) : base_node(), data(data) {}
        ~node() override {}
    };

    base_node *begin_node, *end_node;

    T &get_data(base_node *it) {
        return dynamic_cast<node*>(it)->data;
    }

    T const& get_data(base_node *it) const {
        return dynamic_cast<node*>(it)->data;
    }


public:
    typedef my_iterator<T> iterator;
    typedef my_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    debug_list() : begin_node(), end_node() {
            begin_node = new base_node();
            try {
                end_node = new base_node();
            } catch (...) {
                delete begin_node;
            }
            base_node::connect(begin_node, end_node);
    }

    debug_list (debug_list const& other) : debug_list() {
        for (base_node *i = other.begin_node->prev; i != other.end_node; i = i->prev) {
            push_back(dynamic_cast<node*>(i)->data);
        }
    }

    bool empty() const{
        return begin_node->prev == end_node;
    }

    void clear() {
        while(!empty()) {
            pop_back();
        }
    }

    debug_list& operator=(debug_list const& other) {
        debug_list temp;
        for (base_node *i = other.begin_node->prev; i != other.end_node; i = i->prev) {
            temp.push_back(dynamic_cast<node*>(i)->data);
        }
        clear();
        for (base_node *i = temp.begin_node->prev; i != temp.end_node; i = i->prev) {
            push_back(dynamic_cast<node*>(i)->data);
        }
        return *this;
    }

    iterator begin() {
        return iterator(begin_node->prev, this);
    }

    const_iterator begin() const {
        return const_iterator(begin_node->prev, this);
    }

    iterator end() {
        return iterator(end_node, this);
    }

    const_iterator end() const {
        return const_iterator(end_node, this);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }



    void push_front(T const &new_data) {
        node* temp_node = new node(new_data);
        base_node::connect(temp_node, begin_node->prev);
        base_node::connect(begin_node, temp_node);
    }

    void push_back(T const &new_data) {
        node* temp_node = new node(new_data);
        base_node::connect(end_node->next, temp_node);
        base_node::connect(temp_node, end_node);
    }

    T& front() {
        assert(!empty());
        return  get_data(begin_node->prev);
    }

    T& back() {
        assert(!empty());
        return get_data(end_node->next);
    }

    T const& front() const {
        assert(!empty());
        return get_data(begin_node->prev);
    }

    T const& back() const {
        assert(!empty());
        return get_data(end_node->next);
    }

    void pop_front() {
        assert(!empty());
        base_node *_node = begin_node->prev;
        base_node::connect(begin_node, _node->prev);
        delete _node;
    }

    void pop_back(){
        assert(!empty());
        base_node *_node = end_node->next;
        base_node::connect(_node->next, end_node);
        delete _node;
    }

    void insert(const_iterator it, T const& data) {
        assert(this == it.owner);
        node *_node = new node(data);
        base_node::connect(it._node->next, _node);
        base_node::connect(_node, it._node);
    }

    iterator erase(const_iterator const &it) {
        assert(this == it.owner);
        base_node *temp(it._node->prev);
        base_node::connect(it._node->next, it._node->prev);
        delete it._node;
        return iterator(temp, this);
    }

    void splice(const_iterator pos, debug_list& other, const_iterator first, const_iterator last) {
        assert(pos.owner == this);
        assert(first.owner == &other);
        assert(last.owner == &other);

        bool check = false;

        const_iterator it = first;
        for (;it!=other.end(); ++it) {
            if(it == last) {
                check = true;
                break;
            }
        }
        if(it == other.end()) {
            check = true;
        }
        assert(check);

        check = false;
        for (const_iterator it = first; it!=last; ++it) {
            if(it == pos) {
                check = true;
                break;
            }
        }
        assert(!check);

        if(first == last) return;
        for(const_iterator it = first; it!=last; ++it) {
            it._node->update_owner_iterators(this);
        }

        base_node* begin_path = first._node;
        base_node* end_path = last._node->next;
        base_node::connect(begin_path->next, end_path->prev);

        base_node::connect(pos._node->next, begin_path);
        base_node::connect(end_path, pos._node);

    }

    friend void swap(debug_list & a, debug_list & b) {
        for(typename debug_list<T>::base_node *i = a.begin_node->prev; i != a.end_node; i = i->prev) {
            i->update_owner_iterators(&b);
        }
        for(typename debug_list<T>::base_node *i = b.begin_node->prev; i != b.end_node; i = i->prev) {
            i->update_owner_iterators(&a);
        }
        std::swap(a.begin_node, b.begin_node);
        std::swap(a.end_node, b.end_node);
    }

    ~debug_list() {
        clear();
        delete begin_node;
        delete end_node;
    }

};

template <typename T>
template <typename VT>
class debug_list<T>::my_iterator: public debug_list<T>::any_iterator {
private:
    friend debug_list<T>;
    friend debug_list<T>::base_node;
    base_node *_node;
    bool is_invalid;
    debug_list<T> const* owner;
    typename debug_list<T>::any_iterator *next;

    void invalid() override {
        assert(is_invalid == false);
        is_invalid = true;
    }

    void update_owner(debug_list<T> const* new_owner) override {
        owner = new_owner;
    }

    typename debug_list<T>::any_iterator*& get_next() override {
        return next;
    }

public:
    my_iterator() : _node(nullptr), is_invalid(true), next(nullptr) {}

    my_iterator(my_iterator const& other) :
            _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(!other.is_invalid);
        _node->push_back(this);
    }
/*
    template <typename OTHER_TYPE>
    my_iterator(const my_iterator<OTHER_TYPE> &other,
                typename std::enable_if<std::is_same<typename std::remove_const<VT>::type, OTHER_TYPE>::value>::type * = nullptr)
    : _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(!other.is_invalid);
        _node->push_back(this);
    }
*/
    my_iterator (base_node *_node, debug_list<T> const*_owner)
        : _node(_node), is_invalid(false) {
        owner = _owner;
        next = nullptr;
        _node->push_back(this);
    }

    my_iterator &operator=(my_iterator const& other) {
        assert(!other.is_invalid);
        if (!is_invalid) {
            _node->erase_by_iterator(this);
        }
        _node = other._node;
        is_invalid = other.is_invalid;
        owner = other.owner;
        _node->push_back(this);
        return *this;
    }

    VT& operator*() const{
        assert(!is_invalid);
        assert(_node != owner->end_node);
        assert(_node != owner->begin_node);
        return dynamic_cast<node*>(_node)->data;
    }

    my_iterator& operator++() {
        assert(!is_invalid);
        assert(_node != owner->end_node);
        _node->erase_by_iterator(this);
        _node = _node->prev;
        _node->push_back(this);
        return *this;
    }

    my_iterator operator++(int) {
        assert(!is_invalid);
        assert(_node != owner->end_node);
        my_iterator temp(*this);
        ++(*this);
        return temp;
    }

    my_iterator& operator--() {
        assert(!is_invalid);
        assert(_node != owner->begin_node->prev);
        _node->erase_by_iterator(this);
        _node = _node->next;
        _node->push_back(this);
        return *this;
    }

    my_iterator operator--(int) {
        assert(!is_invalid);
        assert(_node != owner->begin_node->prev);
        my_iterator temp(*this);
        --(*this);
        return temp;
    }

    ~my_iterator() {
        if (!is_invalid) {
            _node->erase_by_iterator(this);
        }
    }

    friend bool operator==(my_iterator const &a, my_iterator const &b)  {
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
        a._node->erase_by_iterator(&a);
        b._node->erase_by_iterator(&b);
        std::swap(a._node, b._node);
        a._node->push_back(&a);
        b._node->push_back(&b);
    }

    typedef std::ptrdiff_t difference_type;
    typedef VT value_type;
    typedef VT* pointer;
    typedef VT& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};

#endif //DEBUG_debug_list_LIST_H
