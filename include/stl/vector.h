/*
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * original author: sillydan1 <https://github.com/sillydan1>
 * */
#ifndef AVRCPP_VECTOR_H
#define AVRCPP_VECTOR_H
#include "default_includes"
#include "../utility"

namespace stl {
    template<typename T>
    class vector {
        static constexpr size_t default_capacity = 1;
    public:
        using iterator = T*;
        using const_iterator = const T*;
        vector();
        explicit vector(unsigned int size);
        explicit vector(int size);
        vector(unsigned int size, const T &initial);
        vector(const vector <T> &v);
        vector(vector<T>&& v) noexcept;
        ~vector();
        auto capacity() const -> unsigned int;
        auto size() const -> unsigned int;
        auto empty() const -> bool;
        auto begin() -> iterator;
        auto begin() const -> iterator;
        auto end() -> iterator;
        auto end() const -> iterator;
        auto front() -> T&;
        auto back() -> T&;
        void push_back(const T& value);
        void emplace_back(T&& value);
        void erase(iterator pos);
        void insert(iterator pos, const T& value);
        void pop_back();
        void reserve(unsigned int new_cap);
        void resize(unsigned int size);
        auto operator[](unsigned int index) const -> T&;
        auto operator=(const vector<T>&) -> vector<T>&;
        auto operator=(vector<T>&&) noexcept -> vector<T>&;
        void clear();
    private:
        unsigned int count{};
        unsigned int max_count{};
        T* data{nullptr};
    };

    template<class T>
    vector<T>::vector()
            : count{0}, max_count{default_capacity}, data{new T[default_capacity]}
    { }

    template<class T>
    vector<T>::vector(const vector<T>& v)
            : count{v.count}, max_count{v.max_count}, data{new T[v.max_count]}
    {
        for (unsigned int i = 0; i < v.count; i++)
            data[i] = v.data[i];
    }

    template<class T>
    vector<T>::vector(vector<T>&& v) noexcept
            : count{v.count}, max_count{v.max_count}, data{v.data}
    {
        v.data = nullptr; // We own the resource now
    }

    template<class T>
    vector<T>::vector(unsigned int size)
            : count{0}, max_count{size}, data{new T[size]}
    { }

    template<class T>
    vector<T>::vector(int size)
            : count{0}, max_count{static_cast<unsigned int>(size)}, data{new T[size]}
    { }

    template<class T>
    vector<T>::vector(unsigned int size, const T& initial)
            : count{size}, max_count{size}, data{new T[size]}
    {
        for (unsigned int i = 0; i < size; i++)
            data[i] = initial;
    }

    template<class T>
    auto vector<T>::operator=(const vector<T>& v) -> vector<T>& {
        if(&v == this)
            return *this;
        delete[] data;
        count = v.count;
        max_count = v.max_count;
        data = new T[count];
        for (unsigned int i = 0; i < count; i++)
            data[i] = v.data[i];
        return *this;
    }

    template<class T>
    auto vector<T>::operator=(vector<T>&& v)  noexcept -> vector<T>& {
        delete[] data;
        count = v.count;
        max_count = v.max_count;
        data = v.data;
        v.data = nullptr; // We own the resource now
        return *this;
    }

    template<class T>
    auto vector<T>::begin() -> typename vector<T>::iterator {
        return data;
    }

    template<class T>
    auto vector<T>::begin() const -> typename vector<T>::iterator {
        return data;
    }

    template<class T>
    auto vector<T>::end() -> typename vector<T>::iterator {
        return data + size();
    }

    template<class T>
    auto vector<T>::end() const -> typename vector<T>::iterator {
        return data + size();
    }

    template<class T>
    auto vector<T>::front() -> T& {
        return data[0];
    }

    template<class T>
    auto vector<T>::back() -> T& {
        return data[count - 1];
    }

    template<class T>
    void vector<T>::push_back(const T &v) {
        if (count >= max_count)
            reserve(max_count << 1u);
        data[count++] = v;
    }

    template<class T>
    void vector<T>::emplace_back(T&& v) {
        if (count >= max_count)
            reserve(max_count << 1u);
        data[count++] = stl::move(v);
    }

    template<class T>
    void vector<T>::pop_back() {
        if(count <= 0)
            return;
        data[count--].~T();
    }

    template<class T>
    void vector<T>::erase(iterator pos) {
        if(pos == end())
            return;
        for(auto i = pos; i + 1 != end(); ++i)
            *i = *(i + 1);
        pop_back();
    }

    template<typename T>
    void vector<T>::insert(iterator pos, const T& value) {
        if(pos == end()) {
            push_back(value);
            return;
        }
        if(count >= max_count)
            reserve(max_count << 1u);

        T v = value;
        count++;
        for(auto it = pos; it != end(); it++) {
            auto cpy = *it;
            *it = v;
            v = cpy;
        }
    }

    template<class T>
    void vector<T>::reserve(unsigned int new_cap) {
        if (data == nullptr) {
            count = 0;
            max_count = 0;
        }
        if(new_cap <= max_count)
            return;
        auto* new_buffer = new T[new_cap];
        for (unsigned int i = 0; i < count; i++)
            new_buffer[i] = data[i];

        max_count = new_cap;
        delete[] data;
        data = new_buffer;
    }

    template<class T>
    auto vector<T>::size() const -> unsigned int {
        return count;
    }

    template<class T>
    void vector<T>::resize(unsigned int size) {
        reserve(size);
        count = size;
    }

    template<class T>
    auto vector<T>::operator[](unsigned int index) const -> T & {
        return data[index];
    }

    template<class T>
    auto vector<T>::capacity() const -> unsigned int {
        return max_count;
    }

    template<class T>
    vector<T>::~vector() {
        delete[] data;
    }

    template<class T>
    void vector<T>::clear() {
        max_count = 0;
        count = 0;
        delete[] data;
        data = nullptr;
    }

    template<class T>
    auto vector<T>::empty() const -> bool {
        return count == 0;
    }
}

#endif //AVRCPP_VECTOR_H
