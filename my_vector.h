#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <list>
#include <memory>
#include <iterator>
#include <exception>
#include <utility>

template <class T>
class my_vector;

template <class T>
class vector_iterator;

template <class T>
class const_vector_iterator: public std::iterator<
    std::random_access_iterator_tag, T> {
 private:
    const my_vector<T>* associated_vector;
    typename std::list<T>::const_iterator inner_iterator;
    typename std::list<std::list<T>>::const_iterator outer_iterator;
    const std::list<T>* current_element_for_inner;

 public:
    friend class my_vector<T>;
    friend class vector_iterator<T>;
    const_vector_iterator() = default;

    const_vector_iterator(const my_vector<T>& other, typename std::list<T>::const_iterator inner, 
        typename std::list<std::list<T>>::const_iterator outer, const std::list<T>& cur_elem)
    {
        associated_vector = &other;
        inner_iterator = inner;
        outer_iterator = outer;
        current_element_for_inner = &cur_elem;
    }

    const_vector_iterator(const my_vector<T>& other, typename std::list<T>::const_iterator inner, 
        typename std::list<std::list<T>>::const_iterator outer)
    {
        associated_vector = &other;
        inner_iterator = inner;
        outer_iterator = outer;
    }

    const_vector_iterator<T>(const vector_iterator<T>& other)
        : associated_vector(other.associated_vector)
        , inner_iterator(other.inner_iterator)
        , outer_iterator(other.outer_iterator)
        , current_element_for_inner(other.current_element_for_inner)
    {}

    const_vector_iterator& operator++() {
        ++inner_iterator;
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            if (outer_iterator != (*associated_vector)._data.end()) {
                current_element_for_inner = &(*outer_iterator);
                inner_iterator = (*outer_iterator).begin();
            }
        }
        return (*this);        
    }

    const_vector_iterator operator++(int) {
        const_vector_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    const_vector_iterator& operator--() {
        if (outer_iterator == (*associated_vector)._data.end()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        } else if (inner_iterator == (*current_element_for_inner).begin() &&
            outer_iterator == (*associated_vector)._data.begin()) {
            --inner_iterator;
        } else if (inner_iterator == (*current_element_for_inner).begin()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        } else {
            --inner_iterator;
        }

        return (*this);
    }

    const_vector_iterator operator--(int) {
        const_vector_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    const_vector_iterator operator+=(size_t shift) {
        while (shift != 0 && inner_iterator != (*current_element_for_inner).end()) {
            ++inner_iterator;
            --shift;
        }
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = &(*outer_iterator);
        }
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = &(*outer_iterator);    
        }
        while (shift != 0) {
            ++inner_iterator;
            --shift;
        }
        return (*this);
    };

    const_vector_iterator operator+ (size_t shift) {
        const_vector_iterator tmp(*this);
        if (shift > 0)
            tmp += shift;
        else 
            tmp -= -shift;
        return tmp;
    }

    const_vector_iterator operator-=(size_t shift) {
        if (outer_iterator == (*associated_vector)._data.end()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
            --shift;
        }
        while (shift != 0 && inner_iterator != (*current_element_for_inner).begin()) {
            --inner_iterator;
            --shift;
        }
        if (shift == 0) {
            return (*this);
        }
        --outer_iterator;
        inner_iterator = --((*outer_iterator).end());
        current_element_for_inner = &(*outer_iterator);
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        }
        while (shift != 0) {
            --shift;
            --inner_iterator;
        }
        return (*this);
    };

    const_vector_iterator operator- (size_t shift) {
        const_vector_iterator tmp(*this);
        if (shift > 0)
            tmp -= shift;
        else 
            tmp += -shift;
        return tmp;
    }

    int operator- (const const_vector_iterator& other) {
        const_vector_iterator tmp(*this);
        int dist1 = 0, dist2 = 0;
        while (tmp.inner_iterator != (*tmp.current_element_for_inner).begin()) {
            --tmp;
            ++dist1;
        }
        while (tmp.outer_iterator != (*associated_vector)._data.begin()) {
            dist1 += (*tmp.outer_iterator).size();
            --tmp.outer_iterator;
        }

        const_vector_iterator tmp2(other);

        while (tmp2.inner_iterator != (*tmp2.current_element_for_inner).begin()) {
            --tmp2;
            ++dist2;
        }
        while (tmp2.outer_iterator != (*associated_vector)._data.begin()) {
            dist2 += (*tmp2.outer_iterator).size();
            --tmp2.outer_iterator;
        }

        return dist1 - dist2;
    }

    const T& operator* () const {
        return *inner_iterator;
    }

    const T& operator->() const {
        return &(*inner_iterator);
    }

    bool operator< (const const_vector_iterator<T>& other) const {
        return other - (*this) > 0;
    }

    bool operator==(const const_vector_iterator<T>& other) const {
        return associated_vector == other.associated_vector &&
            inner_iterator == other.inner_iterator &&
            outer_iterator == other.outer_iterator &&
            (*current_element_for_inner) == (*other.current_element_for_inner);
    }

    bool operator!=(const const_vector_iterator<T>& other) const {
        return !((*this) == other);
    }
};

template <class T>
class vector_iterator: public std::iterator<
    std::random_access_iterator_tag, T>{
 private:
    my_vector<T>* associated_vector;
    typename std::list<T>::iterator inner_iterator;
    typename std::list<std::list<T>>::iterator outer_iterator;
    std::list<T>* current_element_for_inner;

 public:
    friend class my_vector<T>;
    friend class const_vector_iterator<T>;
    vector_iterator() = default;

    vector_iterator(my_vector<T>& other, typename std::list<T>::iterator inner, 
        typename std::list<std::list<T>>::iterator outer, std::list<T>& cur_elem)
    {
        associated_vector = &other;
        inner_iterator = inner;
        outer_iterator = outer;
        current_element_for_inner = &cur_elem;
    }

    vector_iterator(my_vector<T>& other, typename std::list<T>::iterator inner, 
        typename std::list<std::list<T>>::iterator outer)
    {
        associated_vector = &other;
        inner_iterator = inner;
        outer_iterator = outer;
    }

    vector_iterator<T> operator=(const vector_iterator<T>& other) {
        associated_vector = other.associated_vector;
        inner_iterator = other.inner_iterator;
        outer_iterator = other.outer_iterator;
        current_element_for_inner = other.current_element_for_inner;
        return (*this);
    }

    vector_iterator<T> operator=(const const_vector_iterator<T>& other) {
        associated_vector = other.associated_vector;
        inner_iterator = other.inner_iterator;
        outer_iterator = other.outer_iterator;
        current_element_for_inner = other.current_element_for_inner;
        return (*this);
    }

    vector_iterator& operator++() {
        ++inner_iterator;
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            if (outer_iterator != (*associated_vector)._data.end()) {
                current_element_for_inner = &(*outer_iterator);
                inner_iterator = (*outer_iterator).begin();
            }
        }
        return (*this);        
    }

    vector_iterator operator++(int) {
        vector_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    vector_iterator& operator--() {
        if (outer_iterator == (*associated_vector)._data.end()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        } else if (inner_iterator == (*current_element_for_inner).begin() &&
            outer_iterator == (*associated_vector)._data.begin()) {
            --inner_iterator;
        } else if (inner_iterator == (*current_element_for_inner).begin()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        } else {
            --inner_iterator;
        }
        return (*this);
    }

    vector_iterator operator--(int) {
        vector_iterator tmp(*this);
        --(*this);
        return tmp;
    }

    vector_iterator operator+=(size_t shift) {
        while (shift != 0 && inner_iterator != (*current_element_for_inner).end()) {
            ++inner_iterator;
            --shift;
        }
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = &(*outer_iterator);
        }
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            ++outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            current_element_for_inner = &(*outer_iterator);    
        }
        while (shift != 0) {
            ++inner_iterator;
            --shift;
        }
        return (*this);
    };

    vector_iterator operator+ (size_t shift) {
        vector_iterator tmp(*this);
        if (shift > 0)
            tmp += shift;
        else 
            tmp -= -shift;
        return tmp;
    }

    vector_iterator operator-=(size_t shift) {
        if (outer_iterator == (*associated_vector)._data.end()) {
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
            --shift;
        }
        while (shift != 0 && inner_iterator != (*current_element_for_inner).begin()) {
            --inner_iterator;
            --shift;
        }
        if (shift == 0) {
            return (*this);
        }
        --outer_iterator;
        inner_iterator = --((*outer_iterator).end());
        current_element_for_inner = &(*outer_iterator);
        while ((*outer_iterator).size() < shift) {
            shift -= (*outer_iterator).size();
            --outer_iterator;
            inner_iterator = --((*outer_iterator).end());
            current_element_for_inner = &(*outer_iterator);
        }
        while (shift != 0) {
            --inner_iterator;
            --shift;
        }
        return (*this);
    };

    vector_iterator operator- (size_t shift) {
        vector_iterator tmp(*this);
        if (shift > 0)
            tmp -= shift;
        else 
            tmp += -shift;
        return tmp;
    }

    int operator- (const vector_iterator& other) {
        vector_iterator tmp(*this);
        int dist1 = 0, dist2 = 0;
        while (tmp.inner_iterator != (*tmp.current_element_for_inner).begin()) {
            --tmp;
            ++dist1;
        }
        while (tmp.outer_iterator != (*associated_vector)._data.begin()) {
            dist1 += (*tmp.outer_iterator).size();
            --tmp.outer_iterator;
        }

        vector_iterator tmp2(other);

        while (tmp2.inner_iterator != (*tmp2.current_element_for_inner).begin()) {
            --tmp2;
            ++dist2;
        }
        while (tmp2.outer_iterator != (*associated_vector)._data.begin()) {
            dist2 += (*tmp2.outer_iterator).size();
            --tmp2.outer_iterator;
        }

        return dist1 - dist2;
    }

    const T& operator* () const {
        return *inner_iterator;
    }

    const T& operator->() const {
        return &(*inner_iterator);
    }

    T& operator* () {
        return *inner_iterator;
    }

    bool operator< (vector_iterator<T>& other) {
        return other - (*this) > 0;
    }

    bool operator==(const vector_iterator<T>& other) const {
        return associated_vector == other.associated_vector &&
            inner_iterator == other.inner_iterator &&
            outer_iterator == other.outer_iterator;
    }

    bool operator!=(const vector_iterator<T>& other) const {
        return !((*this) == other);
    }
};



template <class T>
class my_vector {
    using size_t = std::size_t;
 private:
    std::list<std::list<T>> _data;
    size_t _size;
    static const size_t _MAX_FOR_ONE_LIST = 64;
    friend class vector_iterator<T>;
    friend class const_vector_iterator<T>;

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
            size_t max_bucket = (*_data.begin()).size();
            size_t min_bucket = (*_data.begin()).size();
            for (const auto& el : _data) {
                max_bucket = std::max(max_bucket, el.size());
                min_bucket = std::min(min_bucket, el.size());
            }
            if (_data.size() == 1 || max_bucket > 2 * min_bucket) {
                size_t bucket_size = floor(sqrt(_size));
                std::list<T> buffer;
                for (const auto& outer : _data) {
                    for (const auto& inner : outer) {
                        buffer.push_back(inner);
                    }
                }
                _data.clear();
                size_t currently_added = 0;
                size_t bigger_buckets = _size % bucket_size;
                for (const auto& el : buffer) {
                    if (currently_added == 0) {
                        _data.emplace_back(std::list<T>());
                    }
                    _data.back().push_back(el);
                    if (bigger_buckets > 0) {
                        if (currently_added == bucket_size + 1) {
                            currently_added = 0;
                            --bigger_buckets;
                        } else {
                            ++currently_added;
                        }
                    } else {
                        currently_added = (currently_added + 1) % bucket_size;
                    }
                }
                buffer.clear();
            }
        }
    }

    void recalc_size() noexcept {
        _size = 0;
        for (const auto& el : _data) {
            _size += el.size();
        }
    }

    void add_back(const T& value) noexcept {
        if (_data.empty()) {
            _data.emplace_back(std::list<T>());
        }
        ++_size;
        _data.back().push_back(value);
    }

    size_t index_by_iterator(const_vector_iterator<T> iter) const {
        size_t index = 0;
        auto cur = (*this).begin();
        while (cur.outer_iterator != iter.outer_iterator) {
            index += ((*cur.outer_iterator).size());
            ++cur.outer_iterator;
            cur.inner_iterator = (*cur.outer_iterator).begin();
            cur.current_element_for_inner = &(*cur.outer_iterator);
        }
        while (cur.inner_iterator != iter.inner_iterator) {
            ++cur.inner_iterator;
            ++index;
        }
        return index;
    }

    vector_iterator<T> build_iterator(size_t index) {
        auto outer_it = _data.begin();
        while ((*outer_it).size() < index && outer_it != _data.end()) {
            index -= (*outer_it).size();
            ++outer_it;
        }
        if (outer_it == _data.end()) {
            throw std::out_of_range("Out of container's bounds");
        }
        auto inner_it = (*outer_it).begin();
        while (index --> 0) {
            ++inner_it;
        }
        return vector_iterator<T>((*this), inner_it, outer_it, (*outer_it));
    }

    const_vector_iterator<T> build_iterator(size_t index) const {
        auto outer_it = _data.begin();
        while ((*outer_it).size() < index && outer_it != _data.end()) {
            index -= (*outer_it).size();
            ++outer_it;
        }
        if (outer_it == _data.end()) {
            throw std::out_of_range("Out of container's bounds");
        }
        auto inner_it = (*outer_it).begin();
        while (index --> 0) {
            ++inner_it;
        }
        return const_vector_iterator<T>((*this), inner_it, outer_it, (*outer_it));
    }

    std::pair<vector_iterator<T>, size_t> get_non_const(const_vector_iterator<T> cpos) {
        size_t index = index_by_iterator(cpos);
        vector_iterator<T> res = build_iterator(index);
        return {res, index};
    }

    T& get_element_by_pos(size_t pos) {
        return (*build_iterator(pos));
    }

    const T& get_element_by_pos(size_t pos) const {
        return (*build_iterator(pos));
    }

    void erase_no_rebalance(const_vector_iterator<T> cpos) {
        auto pos = get_non_const(cpos).first;
        (*pos.outer_iterator).erase(pos.inner_iterator);
    }

 public:
    using iterator = vector_iterator<T>;
    using const_iterator = const_vector_iterator<T>;
    friend class vector_iterator<T>;
    friend class const_vector_iterator<T>;

    explicit my_vector() {
        _size = 0;
    };

    my_vector(size_t count, const T& value = T()) {
        while (count --> 0) {
            add_back(value);
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

    ~my_vector(){};

    my_vector& operator=(const my_vector& other) {
        my_vector<T> tmp(other);
        tmp.swap(*this);
        return (*this);
    };

    my_vector& operator=(std::initializer_list<T> ilist) {
        my_vector<T> tmp(ilist.begin(), ilist.end());
        tmp.swap(*this);
        return (*this);
    };

    my_vector& operator=(my_vector&& other) {
        _data = std::move(other._data);
        _size = std::move(other._size);
        return (*this);
    };

    void assign(size_t count, const T& value) {
        clear();
        while (count --> 0) {
            push_back(value);
        }
        rebalance();
    };

    void assign(std::initializer_list<T> ilist) {
        clear();
        for (auto& el : ilist) {
            add_back(el);
        }
        rebalance();
    };

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
        return build_iterator(0);
    };

    const_iterator begin() const noexcept {
        return build_iterator(0);
    };

    const_iterator cbegin() const noexcept {
        return build_iterator(0);
    };

    iterator end() noexcept {
        return vector_iterator<T>((*this), (*_data.rbegin()).end(), _data.end(), *(--_data.end()));
    };

    const_iterator end() const noexcept {
        return const_vector_iterator<T>((*this), (*_data.rbegin()).end(), _data.end(), *(--_data.end()));
    };

    const_iterator cend() const noexcept {
        return const_vector_iterator<T>((*this), (*_data.rbegin()).end(), _data.end(), *(--_data.end()));
    };

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
        _size = 0;
    };

    iterator insert(iterator pos, const T& value) {
        size_t index = index_by_iterator(pos);
        (*pos.outer_iterator).insert(pos.inner_iterator, value);
        rebalance();
        ++_size;
        return build_iterator(index);
    };

    iterator insert(const_iterator cpos, const T& value) {
        auto tmp = get_non_const(cpos);
        vector_iterator<T>& pos = tmp.first;
        size_t index = tmp.second;
        (*pos.outer_iterator).insert(pos.inner_iterator, value);
        rebalance();
        ++_size;
        return build_iterator(index);
    };

    iterator insert(const_iterator cpos, T&& value) {
        auto tmp = get_non_const(cpos);
        vector_iterator<T>& pos = tmp.first;
        size_t index = tmp.second;
        (*pos.outer_iterator).insert(pos.inner_iterator, std::move(value));
        rebalance();
        ++_size;
        return build_iterator(index);
    };

    iterator insert(iterator pos, size_t count, const T& value) {
        size_t index = index_by_iterator(pos);
        while (count --> 0) {
            (*pos.outer_iterator).insert(pos.inner_iterator, value);
            ++_size;
        }
        rebalance();
        return build_iterator(index);
    };

    template <class input_iterator>
    iterator insert(const_iterator cpos, input_iterator first, input_iterator last) {
        auto tmp = get_non_const(cpos);
        vector_iterator<T>& pos = tmp.first;
        size_t index = tmp.second;
        while (first != last) {
            (*pos.outer_iterator).insert(pos.inner_iterator, (*first));
            ++first;
            ++_size;
        }
        rebalance();
        return build_iterator(index);
    };

    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    };

    iterator erase(iterator first, iterator last) {
        size_t index = index_by_iterator(first);
        if (first.outer_iterator == last.outer_iterator) {
            (*first.outer_iterator).erase(first.inner_iterator, last.inner_iterator);
        } else {
            while (first.inner_iterator != (*first.outer_iterator).end()) {
                first.inner_iterator = (*first.outer_iterator).erase(first.inner_iterator);
            }
            ++first;
            while (first.outer_iterator != last.outer_iterator) {
                auto prev = first.outer_iterator;
                ++first.outer_iterator;
                first.inner_iterator = (*first.outer_iterator).begin();
                first.current_element_for_inner = &(*first.outer_iterator);
                _data.erase(prev);
            }
            while (first.inner_iterator != last.inner_iterator) {
                first.inner_iterator = (*first.outer_iterator).erase(first.inner_iterator);
            }
        }
        recalc_size();
        rebalance();
        return build_iterator(index);
    };

    iterator erase(const_iterator cpos) {
        auto pos = get_non_const(cpos).first;
        return erase(pos, pos + 1);
    };

    template <class... Args>
    iterator emplace(const_iterator cpos, Args&&... args) {
        auto pos = get_non_const(cpos);
        (*pos.first.outer_iterator).emplace(pos.first.inner_iterator, args...);
        ++_size;
        rebalance();
        return build_iterator(pos.second);  
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        _data.back().emplace_back(args...);
        ++_size;
        rebalance();
    }

    void push_back(const T& value) {
        if (_data.empty()) {
            _data.emplace_back(std::list<T>());
        }
        _data.back().push_back(value);
        ++_size;
        rebalance();
    };

    void push_back(T&& value) {
        if (_data.empty()) {
            _data.emplace_back(std::list<T>());
        }
        _data.back().push_back(std::move(value));
        ++_size;
        rebalance();
    };

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
    return !(second > first);
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

