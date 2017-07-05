#include <initializer_list>

template <typename T>
class my_vector {
 private:
    T value_type;

 public:
    using size_t = std::size_t;

    my_vector();

    ~ my_vector();

    my_vector& operator=(const my_vector& other);

    my_vector& operator=(std::initializer_list<T> ilist);

    my_vector& operator=(my_vector&& other);

    void assign(size_t count, const T& value);

    // element access

    T& at(size_t pos);

    const T& at(size_t pos) const;

    T& operator[](size_t pos);

    const T& operator[](size_t pos) const;

    T& front();

    const T& front() const;

    T& back();

    const T& back() const;

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

    bool empty() const noexcept;

    size_t size() const noexcept;

    size_t max_size() const noexcept;

    void reserve(size_t new_capacity);

    size_t capacity() const noexcept;

    void shrink_to_fit();

    // modifiers

    void clear() noexcept;

    iterator insert(iterator pos, const T& value);

    iterator insert(const_iterator pos, const T& value);

    iterator insert(const_iterator pos, T&& value);

    iterator insert(iterator pos, size_t count, const T& value);

    template <class input_iterator>
    iterator insert(const_iterator pos, input_iterator first, input_iterator last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    iterator erase(const_iterator pos);

    iterator erase(iterator first, iterator last);

    void push_back(const T& value);

    void push_back(T&& value);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <class... Args>
    Args& emplace_back(Args&&... args);

    void pop_back();

    void resize(size_t new_size);

    void resize(size_t new_size, const T& value);

    void swap(my_vector& other);

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