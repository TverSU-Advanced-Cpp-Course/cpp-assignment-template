#include <iterator>
#include <type_traits>

template <typename T>
void construct(T* p, const T& rhs = T()) {
    new (p) T(rhs);
}

template <typename T>
void destroy(T* p) noexcept {
    p->~T();
}

template <typename T>
class Vector {
private:
    size_t size_;
    size_t capacity_;
    T* arr_;

public:
    template <bool IsConst>
    class base_iterator {
    public:
        using value_type = T;
        using reference = std::conditional_t<IsConst, const T&, T&>;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

    private:
        pointer ptr_;

    public:
        base_iterator() : ptr_(nullptr) {}
        explicit base_iterator(pointer ptr) : ptr_(ptr) {}
        template <bool OtherConst, typename = std::enable_if_t<IsConst && !OtherConst>>
        base_iterator(const base_iterator<OtherConst>& other) : ptr_(other.ptr_) {}

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        reference operator[](difference_type n) const {
            return ptr_[n];
        }

        base_iterator& operator++() {
            ++ptr_;
            return *this;
        }

        base_iterator operator++(int) {
            base_iterator tmp = *this;
            ++ptr_;
            return tmp;
        }

        base_iterator& operator--() {
            --ptr_;
            return *this;
        }

        base_iterator operator--(int) {
            base_iterator tmp = *this;
            --ptr_;
            return tmp;
        }

        base_iterator& operator+=(difference_type n) {
            ptr_ += n;
            return *this;
        }

        base_iterator& operator-=(difference_type n) {
            ptr_ -= n;
            return *this;
        }

        base_iterator operator+(difference_type n) const {
            return base_iterator{ptr_ + n};
        }

        base_iterator operator-(difference_type n) const {
            return base_iterator{ptr_ - n};
        }

        friend base_iterator operator+(difference_type n, const base_iterator& it) {
            return base_iterator{it.ptr_ + n};
        }

        template <bool OtherConst>
        difference_type operator-(const base_iterator<OtherConst>& other) const {
            return ptr_ - other.ptr_;
        }

        template <bool OtherConst>
        auto operator<=>(const base_iterator<OtherConst>& other) const {
            return ptr_ <=> other.ptr_;
        }

        template <bool OtherConst>
        bool operator==(const base_iterator<OtherConst>& other) const {
            return ptr_ == other.ptr_;
        }

    private:
        template <bool>
        friend class base_iterator;
    };

    using iterator = base_iterator<false>;
    using const_iterator = base_iterator<true>;

    iterator begin() {
        return iterator{arr_};
    }

    iterator end() {
        return iterator{arr_ + size_};
    }

    const_iterator begin() const {
        return const_iterator{arr_};
    }

    const_iterator end() const {
        return const_iterator{arr_ + size_};
    }

    const_iterator cbegin() const {
        return const_iterator{arr_};
    }

    const_iterator cend() const {
        return const_iterator{arr_ + size_};
    }

    Vector(size_t sz = 0)
        : size_(0),
          capacity_(sz),
          arr_((sz == 0) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * sz))) {}

    Vector(std::initializer_list<T> init) : Vector(init.size()) {
        for (const T& value : init) {
            construct(arr_ + size_, value);
            ++size_;
        }
    }

    ~Vector() {
        for (size_t i = 0; i < size_; ++i) {
            destroy(arr_ + i);
        }

        ::operator delete(arr_);
    }

    Vector(const Vector& rhs) : Vector(rhs.size_) {
        while (size_ < rhs.size_) {
            construct(arr_ + size_, rhs.arr_[size_]);
            ++size_;
        }
    }

    Vector& operator=(const Vector& rhs) {
        Vector tmp(rhs);
        std::swap(*this, tmp);
        return *this;
    }

    Vector(Vector&& rhs) noexcept : size_(rhs.size_), capacity_(rhs.capacity_), arr_(rhs.arr_) {
        rhs.size_ = 0;
        rhs.capacity_ = 0;
        rhs.arr_ = nullptr;
    }

    Vector& operator=(Vector&& rhs) noexcept {
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(arr_, rhs.arr_);
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            Vector tmp(capacity_ > 0 ? capacity_ * 2 : 1);
            while (tmp.size_ < size_) {
                tmp.push_back(arr_[tmp.size_]);
            }
            tmp.push_back(value);
            std::swap(*this, tmp);
            return;
        }

        construct(arr_ + size_, value);
        ++size_;
    }

    void pop_back() {
        --size_;
        destroy(arr_ + size_);
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    T& operator[](size_t idx) {
        return arr_[idx];
    }

    const T& operator[](size_t idx) const {
        return arr_[idx];
    }

    void resize(size_t size) {
        if (size > capacity_) {
            reserve(size);
        }

        while (size > size_) {
            construct(arr_ + size_);
            size_++;
        }

        while (size < size_) {
            pop_back();
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        T* new_arr = static_cast<T*>(::operator new(sizeof(T) * new_capacity));

        for (size_t i = 0; i < size_; ++i) {
            construct(new_arr + i, arr_[i]);
            destroy(arr_ + i);
        }

        ::operator delete(arr_);

        arr_ = new_arr;
        capacity_ = new_capacity;
    }
};
