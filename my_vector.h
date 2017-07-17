#include <initializer_list>
#include <list>
#include <memory>
#include <iterator>
#include <exception>
#include <utility>

template <typename T>
class vector_iterator: public std::iterator<
    std::random_access_iterator_tag, T> {
 private:
    friend class my_vector<T>;
    const shared_ptr<my_vector<T>> associated_vector;
    std::list<T>::iterator inner_iterator;
    std::list<std::list<T>>::iterator outer_iterator;
    list<T>& current_element_for_inner;

 public:
    // TODO : +=, -=
    vector_iterator() = default;

    vector_iterator(const my_vector<T>& other, std::list<T>::iterator inner, 
        std::list<std::list<T>>::iterator outer, const std::list<T>& cur_elem)
        : associated_vector(make_shared<my_vector<T>>(other))
        , inner_iterator(inner)
        , outer_iterator(outer)
        , current_element_for_inner(cur_elem)
    {}

    vector_iterator& operator++() {
        if (inner_iterator == (*current_element_for_inner).end()) {
            ++outer_iterator;
            current_element_for_inner = *outer_iterator;
            inner_iterator = (*outer_iterator).begin();
            return (*this);
        } else {
            ++inner_iterator;
            return (*this);
        }
    }

    vector_iterator operator++(int) {
        vector_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    vector_iterator& operator--() {
        if (inner_iterator == (*current_element_for_inner).begin()
            && outer_iterator == (*associated_vector).begin()) {
            --inner_iterator;
            return (*this);
        } else if (inner_iterator == (*current_element_for_inner).begin()) {
            --outer_iterator;
            inner_iterator = --(*outer_iterator).end();
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

    const T& operator* () const {
        return *inner_iterator;
    }

    const T& operator->() const {
        return &(*inner_iterator);
    }
};


template <typename T>
class my_vector {
 private:
    using list = std::list;
    using size_t = std::size_t;
    using literator = std::list<T>::iterator;
    list<list<T>> _data;
    size_t _size;
    const size_t _MAX_FOR_ONE_LIST = 128;
    friend class vector_iterator<T>;

    void rebalance() noexcept { // TODO : deletion of empty blocks
        if (_size <= _MAX_FOR_ONE_LIST) {
            // TODO
            if (_data.size() == 1) {
                return;
            }

        } else {
            // TODO
        }
    }

    void add_back(T& value) noexcept {
        if (_data.empty()) {
            _data.push_back(list<T>());
        }
        _data.back().push_back(value);
    }

    std::pair<const std::list<T>&, size_t> get_bucket_by_pos(size_t pos) const {
        literator it = _data.begin();
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
            throw std::out_of_range();
        }
    };

    T& get_element_by_pos(size_t pos) const {
        auto tmp = get_bucket_by_pos(pos);
        pos -= it.second;
        literator it = tmp.first.begin();
        while (pos --> 0) {
            ++it;
        }
        return (*it);
    }

 public:
    using iterator = vector_iterator;
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

    my_vector(std::initializer_list<T>& ilist)
        : my_vector(ilist.begin(), ilist.end())
    {};

    my_vector(const my_vector& other)
        : my_vector(other.begin(), other.end())
    {};

    ~ my_vector();

    my_vector& operator=(const my_vector& other);

    my_vector& operator=(std::initializer_list<T> ilist);

    my_vector& operator=(my_vector&& other);

    void assign(size_t count, const T& value);

    void assign(std::initializer_list<T> ilist);

    // element access

    T& at(size_t pos) {
        if (!(pos < _size)) {
            throw std::out_of_range();
        }
        return get_element_by_pos(pos);
    };

    const T& at(size_t pos) const {
        if (!(pos < _size)) {
            throw std::out_of_range();
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
            throw std::out_of_range();
        }
        return _data.front().front();
    };

    const T& front() const {
        if (_data.empty() || _data.front().empty()) {
            throw std::out_of_range();
        }
        return _data.front().front();
    };

    T& back() {
        if (_data.empty() || _data.back().empty()) {
            throw std::out_of_range();
        }
        return _data.back().back();
    };

    const T& back() const {
        if (_data.empty() || _data.back().empty()) {
            throw std::out_of_range();
        }
        return _data.back().back();
    };

    // iterators

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

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

    size_t max_size() const noexcept = delete;

    void reserve(size_t new_capacity) = delete;

    size_t capacity() const noexcept = delete;

    void shrink_to_fit() = delete();

    // modifiers

    void clear() noexcept {
        while (!_data.empty()) {
            _data.back().clear();
            _data.pop_back();
        }
    };

    iterator insert(iterator pos, const T& value);

    iterator insert(const_iterator pos, const T& value);

    iterator insert(const_iterator pos, T&& value);

    iterator insert(iterator pos, size_t count, const T& value);

    template <class input_iterator>
    iterator insert(const_iterator pos, input_iterator first, input_iterator last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    iterator erase(const_iterator pos);

    iterator erase(iterator first, iterator last);

    void push_back(const T& value) {
        if (_data.empty()) {
            _data.push_back(std::list<T>());
        }
        _data.back().push_back(value);
        ++_size;
        rebalance();
    };

    void push_back(T&& value);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <class... Args>
    Args& emplace_back(Args&&... args);

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

    void resize(size_t new_size) = delete;

    void resize(size_t new_size, const T& value) = delete;

    void swap(my_vector& other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
    };

};

template<class T>
bool operator==(my_vector<T>& first, my_vector<T>& second);

template<class T>
bool operator!=(my_vector<T>& first, my_vector<T>& second);

template<class T>
bool operator<(my_vector<T>& first, my_vector<T>& second);

template<class T>
bool operator<=(my_vector<T>& first, my_vector<T>& second);

template<class T>
bool operator>(my_vector<T>& first, my_vector<T>& second);

template<class T>
bool operator>=(my_vector<T>& first, my_vector<T>& second);

template <class T>
void swap(my_vector<T>& left, my_vector<T>& right) {
    left.swap(right);
}