# Итератор для вектора

## Описание
Вам дан упрощённый шаблон класса `Vector<T>`, реализующий базовый интерфейс динамического массива. Ваша задача --- добавить в этот класс поддержку итераторов.

Необходимо реализовать вложенные типы `iterator` и `const_iterator`, а также методы `begin()`, `end()`, `cbegin()`, `cend()`.

## Требования к реализации

Класс `Vector` должен содержать публичные типы `iterator` и `const_iterator`. Оба итератора должны быть реализованы в публичной секции класса `Vector` и соответствовать категории Random Access Iterator.

Каждый итератор должен иметь следующие подтипы:
- `value_type` --- `T`
- `reference` --- `T&` для `iterator`, `const T&` для `const_iterator`
- `pointer` --- `T*` для `iterator`, `const T*` для `const_iterator`
- `difference_type` --- `std::ptrdiff_t`
- `iterator_category` --- `std::random_access_iterator_tag`

Также нужно реализовать все операции для поддержки интерфейса Random Access итератора:
- Разыменование: `*it`, `it->`, `it[n]`
- Инкремент/декремент: `++it`, `it++`, `--it`, `it--`
- Арифметика: `it + n`, `n + it`, `it - n`, `it += n`, `it -= n`
- Разность итераторов: `it1 - it2`
- Сравнения: `==`, `!=`, `<`, `>`, `<=`, `>=`

Ко всему прочему, `iterator` должен уметь неявно преобразовываться в `const_iterator`.

Для класса `Vector` нужно будет добавить методы получения итераторов:
- `cbegin()` --- возвращает `const_iterator` на первый элемент
- `cend()` --- возвращает `const_iterator` на элемент за последним
- `begin()` --- возвращает `iterator` на первый элемент (для константного вектора возвращает `const_iterator`)
- `end()` --- возвращает `iterator` на элемент за послденим (для константного вектора возвращает `const_iterator`)


После реализации методов, класс `Vector` будет представлять собой диапазон и должен уметь работать с Range-Based for и стандартными алгоритмами.

## Шаблон кода
Вам предоставлен следующий код класса `Vector`. Добавьте в него поддержку итераторов, не изменяя существующий функционал.

```c++
#pragma once
#include <cstddef>
#include <initializer_list>
#include <utility>

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
    Vector(size_t sz = 0)
        : size_(0)
        , capacity_(sz)
        , arr_((sz == 0) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * sz))) {}

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

    Vector(Vector&& rhs) noexcept
            : size_(rhs.size_)
            , capacity_(rhs.capacity_)
            , arr_(rhs.arr_) {
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
```

## Формат сдачи
Вам необходимо написать только заголовочный файл. Не добавляйте в него функцию `main`. Тестирующая система подключит ваш файл через `#include`.

## Примеры использования
```c++
#include <iostream>
#include <algorithm>
#include <numeric>

int main() {
    Vector<int> v = {1, 2, 3, 4, 5};

    // Range-Based for
    for (int x : v) {
        std::cout << x << " "; // 1 2 3 4 5
    }
    std::cout << "\n";

    // Модификация через итератор
    for (auto it = v.begin(); it != v.end(); ++it) {
        *it *= 2;
    }
    // v = {2, 4, 6, 8, 10}

    // Константный вектор
    const Vector<int>& cv = v;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        std::cout << *it << " "; // 2 4 6 8 10
    }
    std::cout << "\n";

    // cbegin/cend
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Random access операции
    auto it = v.begin();
    std::cout << it[2] << "\n";     // 6
    std::cout << *(it + 3) << "\n"; // 8

    // Разность итераторов
    std::cout << (v.end() - v.begin()) << "\n"; // 5

    // Сравнения
    std::cout << (v.begin() < v.end()) << "\n"; // 1

    // Стандартные алгоритмы
    std::sort(v.begin(), v.end(), std::greater<int>());
    // v = {10, 8, 6, 4, 2}

    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << sum << "\n"; // 30

    auto found = std::find(v.begin(), v.end(), 6);
    if (found != v.end()) {
        std::cout << "Found: " << *found << "\n"; // Found: 6
    }

    // Преобразование iterator -> const_iterator
    Vector<int>::const_iterator cit = v.begin();
    std::cout << *cit << "\n"; // 10
}
```
