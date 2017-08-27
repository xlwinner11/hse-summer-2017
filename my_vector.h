#include <initializer_list>
#include <list>
#include <memory>
#include <iterator>
#include <exception>
#include <utility>

template <class T>
class my_vector;

template <class T>
class vector_iterator: public std::iterator<
    std::random_access_iterator_tag, T> {
 private:
    // friend class my_vector<T>;
    const std::shared_ptr<my_vector<T>> associated_vector;
    typename std::list<T>::iterator inner_iterator;
    typename std::list<std::list<T>>::iterator outer_iterator;
    std::list<T>& current_element_for_inner;

 public:
    vector_iterator() = default;

    vector_iterator(const my_vector<T>& other, typename std::list<T>::iterator inner, 
        typename std::list<std::list<T>>::iterator outer, const std::list<T>& cur_elem)
        : associated_vector(std::make_shared<my_vector<T>>(other))
        , inner_iterator(inner)
        , outer_iterator(outer)
        , current_element_for_inner(cur_elem)
    {}

    vector_iterator& operator++() {
        ++inner_iterator;
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            current_element_for_inner = *outer_iterator;
            inner_iterator = (*outer_iterator).begin();
        }
        return (*this);        
    }

    vector_iterator operator++(int) {
        vector_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    vector_iterator& operator--() {
        if (inner_iterator == current_element_for_inner.begin()
            && outer_iterator == (*associated_vector).begin()) {
            --inner_iterator;
            return (*this);
        } else if (inner_iterator == current_element_for_inner.begin()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = *outer_iterator;
            return (*this);
        } else {
            --inner_iterator;
            return (*this);
        }
    }

    vector_iterator operator--(int) {
        vector_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    vector_iterator operator+=(size_t shift) {
        while (shift --> 0 && inner_iterator != (*current_element_for_inner).end()) {
            ++inner_iterator;
        }
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = (*outer_iterator);
        }
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = (*outer_iterator);    
        }
        while (shift --> 0) {
            ++inner_iterator;
        }
        return (*this);
    };

    vector_iterator operator-=(size_t shift) {
        while (shift --> 0 && inner_iterator != (*current_element_for_inner).begin()) {
            --inner_iterator;
        }
        if (shift == 0) {
            return (*this);
        }
        --outer_iterator;
        inner_iterator = --((*outer_iterator).end());
        current_element_for_inner = *outer_iterator;
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = *outer_iterator;
        }
        while (shift --> 0) {
            --inner_iterator;
        }
        return (*this);
    };

    const T& operator* () const {
        return *inner_iterator;
    }

    const T& operator->() const {
        return &(*inner_iterator);
    }

    bool operator==(vector_iterator<T>& other) const {
        return associated_vector == other.associated_vector &&
            inner_iterator == other.inner_iterator &&
            outer_iterator == other.outer_iterator &&
            current_element_for_inner == other.current_element_for_inner;
    }
};

template <class T>
class const_vector_iterator: public std::iterator<
    std::random_access_iterator_tag, T> {
 private:
    // friend class my_vector<T>;
    const std::shared_ptr<my_vector<T>> associated_vector;
    typename std::list<T>::const_iterator inner_iterator;
    typename std::list<std::list<T>>::const_iterator outer_iterator;
    std::list<T>& current_element_for_inner;

 public:
    const_vector_iterator() = default;

    const_vector_iterator(const my_vector<T>& other, typename std::list<T>::const_iterator inner, 
        typename std::list<std::list<T>>::const_iterator outer, const std::list<T>& cur_elem)
        : associated_vector(std::make_shared<my_vector<T>>(other))
        , inner_iterator(inner)
        , outer_iterator(outer)
        , current_element_for_inner(cur_elem)
    {}

    const_vector_iterator& operator++() {
        ++inner_iterator;
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            current_element_for_inner = *outer_iterator;
            inner_iterator = (*outer_iterator).begin();
        }
        return (*this);        
    }

    const_vector_iterator operator++(int) {
        const_vector_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    const_vector_iterator& operator--() {
        if (inner_iterator == current_element_for_inner.begin()
            && outer_iterator == (*associated_vector).begin()) {
            --inner_iterator;
            return (*this);
        } else if (inner_iterator == current_element_for_inner.begin()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = *outer_iterator;
            return (*this);
        } else {
            --inner_iterator;
            return (*this);
        }
    }

    const_vector_iterator operator--(int) {
        const_vector_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    const_vector_iterator operator+=(size_t shift) {
        while (shift --> 0 && inner_iterator != (*current_element_for_inner).end()) {
            ++inner_iterator;
        }
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = (*outer_iterator);
        }
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = (*outer_iterator);    
        }
        while (shift --> 0) {
            ++inner_iterator;
        }
        return (*this);
    };

    const_vector_iterator operator-=(size_t shift) {
        while (shift --> 0 && inner_iterator != (*current_element_for_inner).begin()) {
            --inner_iterator;
        }
        if (shift == 0) {
            return (*this);
        }
        --outer_iterator;
        inner_iterator = --((*outer_iterator).end());
        current_element_for_inner = *outer_iterator;
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = *outer_iterator;
        }
        while (shift --> 0) {
            --inner_iterator;
        }
        return (*this);
    };

    const T& operator* () const {
        return *inner_iterator;
    }

    const T& operator->() const {
        return &(*inner_iterator);
    }

    bool operator==(const_vector_iterator<T>& other) const {
        return associated_vector == other.associated_vector &&
            inner_iterator == other.inner_iterator &&
            outer_iterator == other.outer_iterator &&
            current_element_for_inner == other.current_element_for_inner;
    }
};

template <class T>
class my_vector {
    using size_t = std::size_t;
    using iterator = vector_iterator<T>;
    using const_iterator = const_vector_iterator<T>;
 private:
    std::list<std::list<T>> _data;
    size_t _size;
    const size_t _MAX_FOR_ONE_LIST = 128;
    friend class vector_iterator<T>;

    void rebalance() noexcept {
        if (_size <= _MAX_FOR_ONE_LIST) {
            if (_data.size() == 1) {
                return;
            } else {
                auto it = _data.begin();
                ++it;
                for (; it != _data.end(); ++it) {
                    auto cur_list = *it;
                    for (auto& el : cur_list) {
                        (*_data.begin()).push_back(el);
                    }
                }
                while (_data.size() > 1) {
                    _data.pop_back();
                }
            }
        } else {
            // TODO
        }
    }

    void add_back(T& value) noexcept {
        if (_data.empty()) {
            _data.push_back(std::list<T>());
        }
        _data.back().push_back(value);
    }

    std::pair<const std::list<T>&, size_t> get_bucket_by_pos(size_t pos) const {
        auto it = _data.begin();
        size_t current_size = 0;
        while (it != _data.end()) {
            if ((*it).size() + current_size < pos) {
                ++it;
                current_size += (*it).size();
            } else {
                return {(*it), current_size};
            }
        }
        if (it == _data.end()) {
            throw std::out_of_range("Out of container's bounds");
        }
    };

    size_t index_by_iterator(iterator iter) const {
        size_t index = 0;
        iterator cur = (*this).begin();
        while (cur.outer_iterator != iter.outer_iterator) {
            index += ((*cur.outer_iterator).size());
            ++cur.outer_iterator;
            cur.inner_iterator = (*cur.outer_iterator).begin();
            cur.current_element_for_inner = (*cur.outer_iterator);
        }
        while (cur.inner_iterator != iter.inner_iterator) {
            ++cur.inner_iterator;
            ++index;
        }
        return index;
    }

    T& get_element_by_pos(size_t pos) const {
        auto tmp = get_bucket_by_pos(pos);
        pos -= tmp.second;
        auto it = tmp.first.begin();
        while (pos --> 0) {
            ++it;
        }
        return (*it);
    }

    vector_iterator<T> build_iterator(size_t index) {
        vector_iterator<T> cur = (*this).begin();
        while ((*cur.outer_iterator).size() < index) {
            index -= (*cur.outer_iterator).size();
            ++cur.outer_iterator;
            cur.inner_iterator = (*cur.outer_iterator).begin();
            cur.current_element_for_inner = (*cur.outer_iterator);    
        }
        while (index --> 0) {
            ++cur.inner_iterator;
        }
        return cur;
    }

    void erase_no_rebalance(iterator pos) {
        (*pos.outer_iterator).erase(pos.inner_iterator);
    }

 public:
    explicit my_vector() = default;

    explicit my_vector(size_t count, const T& value = T()) {
        while (count --> 0) {
            add_back(count);
        }
        rebalance();
    }

    template <typename RAIterator>
    my_vector(RAIterator first, RAIterator last) {
        while (first != last) {
            add_back(*first);
            ++first;
            ++_size;
        }
        rebalance();
    }

    my_vector(std::initializer_list<T> ilist)
        : my_vector(ilist.begin(), ilist.end())
    {};

    my_vector(const my_vector& other)
        : my_vector(other.begin(), other.end())
    {};

    ~ my_vector(){};

    my_vector& operator=(const my_vector& other);

    my_vector& operator=(std::initializer_list<T> ilist);

    my_vector& operator=(my_vector&& other);

    void assign(size_t count, const T& value) {
        clear();
        while (count --> 0) {
            push_back(value);
        }
    };

    void assign(std::initializer_list<T> ilist);

    // element access

    T& at(size_t pos) {
        if (!(pos < _size)) {
            throw std::out_of_range("Wrong index");
        }
        return get_element_by_pos(pos);
    };

    const T& at(size_t pos) const {
        if (!(pos < _size)) {
            throw std::out_of_range("Wrong index");
        }
        return get_element_by_pos(pos);
    };

    T& operator[](size_t pos) {
        return get_element_by_pos(pos);
    };

    const T& operator[](size_t pos) const {
        return get_element_by_pos(pos);
    };

    T& front() {
        if (_data.empty() || _data.front().empty()) {
            throw std::out_of_range("Empty container");
        }
        return _data.front().front();
    };

    const T& front() const {
        if (_data.empty() || _data.front().empty()) {
            throw std::out_of_range("Empty container");
        }
        return _data.front().front();
    };

    T& back() {
        if (_data.empty() || _data.back().empty()) {
            throw std::out_of_range("Empty container");
        }
        return _data.back().back();
    };

    const T& back() const {
        if (_data.empty() || _data.back().empty()) {
            throw std::out_of_range("Empty container");
        }
        return _data.back().back();
    };

    // iterators

    iterator begin() noexcept {
        return vector_iterator<T>(this, (*_data.begin()).begin(), _data.begin(), (*_data.begin()));
    };

    const_iterator begin() const noexcept {
        return vector_iterator<T>(this, (*_data.begin()).cbegin(), _data.cbegin(), (*_data.begin()));
    };

    const_iterator cbegin() const noexcept {
        return vector_iterator<T>(this, (*_data.begin()).cbegin(), _data.cbegin(), (*_data.begin()));
    };

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    iterator rbegin() noexcept;

    const_iterator rbegin() const noexcept;

    const_iterator crbegin() const noexcept;

    iterator rend() noexcept;

    const_iterator rend() const noexcept;

    const_iterator crend() const noexcept;

    // capacity

    bool empty() const noexcept {
        return _size == 0;
    };

    size_t size() const noexcept {
        return _size;
    };

    // modifiers

    void clear() noexcept {
        while (!_data.empty()) {
            _data.back().clear();
            _data.pop_back();
        }
    };

    iterator insert(iterator pos, const T& value) {
        size_t index = index_by_iterator(pos);
        (*pos.outer_iterator).insert(pos.inner_iterator, value);
        rebalance();
        return build_iterator(index);
    };

    iterator insert(const_iterator pos, const T& value) {
        size_t index = index_by_iterator(pos);
        (*pos.outer_iterator).insert(pos.inner_iterator, value);
        rebalance();
        return build_iterator(index);
    };

    iterator insert(const_iterator pos, T&& value) {
        size_t index = index_by_iterator(pos);
        (*pos.outer_iterator).insert(pos.inner_iterator, value);
        rebalance();
        return build_iterator(index);
    };

    iterator insert(iterator pos, size_t count, const T& value) {
        size_t index = index_by_iterator(pos);
        while (count --> 0) {
            (*pos.outer_iterator).insert(pos.inner_iterator, value);
        }
        rebalance();
        return build_iterator(index);
    };

    template <class input_iterator>
    iterator insert(const_iterator pos, input_iterator first, input_iterator last) {
        size_t index = index_by_iterator(pos);
        while (first != last) {
            (*pos.outer_iterator).insert(pos.inner_iterator, (*first));
            ++first;
        }
        rebalance();
        return build_iterator(index);
    };

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    };

    iterator erase(const_iterator pos) {
        size_t index = index_by_iterator(pos);
        (*pos.outer_iterator).erase(pos.inner_iterator);
        rebalance();
        return build_iterator(index);
    };

    iterator erase(iterator first, iterator last) {
        size_t index = index_by_iterator(first);
        while (first != last) {
            erase_no_rebalance(first);
            ++first;
        }
        rebalance();
        return build_iterator(index);
    };

    void push_back(const T& value) {
        if (_data.empty()) {
            _data.push_back(std::list<T>());
        }
        _data.back().push_back(value);
        ++_size;
        rebalance();
    };

    void push_back(T&& value);

    void pop_back() {
        if (!_data.empty()) {
            _data.back().pop_back();
            if (_data.back().empty()) {
                _data.pop_back();
            }
            --_size;
            rebalance();
        }
    };

    void swap(my_vector& other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
    };

};

template<class T>
bool operator==(my_vector<T>& first, my_vector<T>& second) {
    if (first.size() != second.size()) {
        return false;
    }
    auto it1 = first.begin(), it2 = second.begin();
    while (it1 != first.end()) {
        if ((*it1) != (*it2)) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
};

template<class T>
bool operator!=(my_vector<T>& first, my_vector<T>& second) {
    return !(first == second);
};

template<class T>
bool operator<(my_vector<T>& first, my_vector<T>& second) {
    auto it1 = first.begin(), it2 = second.begin();
    while (it1 != first.end() && it2 != second.end()) {
        if ((*it1) < (*it2)) {
            return true;
        } else if ((*it1) == (*it2)) {
            return false;
        }
        ++it1;
        ++it2;
    }
    if (it1 == first.end()) {
        return true;
    } else {
        return false;
    }
};

template<class T>
bool operator<=(my_vector<T>& first, my_vector<T>& second) {
    if (first == second) {
        return true;
    }
    return first < second;
};

template<class T>
bool operator>(my_vector<T>& first, my_vector<T>& second) {
    return !(first <= second);
};

template<class T>
bool operator>=(my_vector<T>& first, my_vector<T>& second) {
    return !(first < second);
}

template <class T>
void swap(my_vector<T>& left, my_vector<T>& right) {
    left.swap(right);
}
