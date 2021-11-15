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
#ifndef AVRCPP_DEQUE_H
#define AVRCPP_DEQUE_H
#include "iterators.h"
#ifndef AVRCPP_DEFAULT_DEQUE_CHUNK_SIZE
// Note: this is the ELEMENT AMOUNT in a deque buffer - not byte size
#define AVRCPP_DEFAULT_DEQUE_CHUNK_SIZE 10
#endif

namespace stl {
    template<typename T, size_t _deque_chunk_size = AVRCPP_DEFAULT_DEQUE_CHUNK_SIZE>
    class deque {
    public:
        using value_type = T;
        using map_pointer = value_type**;
        using reference = value_type&;
        using pointer = value_type*;
        using const_reference = const value_type&;
        using size_type = size_t;
        using iterator = _deque_iterator<value_type, _deque_chunk_size>;

        deque();
        ~deque();
        [[nodiscard]] inline auto size() const -> size_type;
        [[nodiscard]] inline auto empty() const -> bool;
        auto begin() -> iterator;
        auto begin() const -> iterator;
        auto end() -> iterator;
        auto end() const -> iterator;
        auto front() -> reference;
        auto front() const -> const_reference;
        auto back() -> reference;
        auto back() const -> const_reference;

        void clear();
        void push_back(const_reference v);
        void emplace_back(value_type&& v);
        void pop_back();
        void push_front(const_reference v);
        void emplace_front(value_type&& v);
        void pop_front();
    private:
        void destroy_range(iterator a, iterator b);
        void destroy_range(pointer a, pointer b);

        map_pointer map;
        size_type map_size;
        iterator start;
        iterator finish;
    };

    template<typename T, size_t deque_chunk_size>
    deque<T, deque_chunk_size>::deque()
     : map{nullptr}, map_size{}, start{nullptr}, finish{nullptr}
    {
        auto* node0 = (pointer)malloc(deque_chunk_size * sizeof(value_type));
        map = (map_pointer)malloc(1 * sizeof(pointer));
        map[0] = node0;
        map_size = 1;
        start = iterator{map};
        finish = iterator{map};
    }

    template<typename T, size_t deque_chunk_size>
    deque<T, deque_chunk_size>::~deque() {
        destroy_range(start, finish);
        free(map);
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::size() const -> deque<T, deque_chunk_size>::size_type {
        return finish - start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::empty() const -> bool {
        return finish == start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::begin() -> deque<T, deque_chunk_size>::iterator {
        return start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::begin() const -> deque<T, deque_chunk_size>::iterator {
        return start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::end() -> deque<T, deque_chunk_size>::iterator {
        return finish;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::end() const -> deque<T, deque_chunk_size>::iterator {
        return finish;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::front() -> reference {
        return *start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::front() const -> const_reference {
        return *start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::back() -> reference {
        return *(finish-1);
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::back() const -> const_reference {
        return *(finish-1);
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::clear() {
        if(empty())
            return;
        // Destroy all "middle" nodes
        if(map_size > 2) {
            for (map_pointer node = start.node + 1; node < finish.node; ++node)
                destroy_range(*node, (*node) + deque_chunk_size);
        }
        // Destroy "end" nodes
        if(start.node != finish.node) {
            destroy_range(start.current, start.last);
            destroy_range(finish.first, finish.current);
        } else
            destroy_range(start.current, finish.current);
        finish = start;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::push_back(const_reference v) {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::emplace_back(value_type&& v) {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::pop_back() {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::push_front(const_reference v) {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::emplace_front(value_type&& v) {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::pop_front() {

    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::destroy_range(deque<T, deque_chunk_size>::iterator a, deque<T, deque_chunk_size>::iterator b) {
        for(auto p = a; p != b; ++p)
            p.current->~T();
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::destroy_range(deque<T, deque_chunk_size>::pointer a, deque<T, deque_chunk_size>::pointer b) {
        for(auto p = a; p != b; ++p)
            p->~T();
    }
}

#endif //AVRCPP_DEQUE_H
