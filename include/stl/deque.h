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
#include "algorithm.h"
#include "iterators.h"
#include "default_includes"
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
        inline auto size() const -> size_type;
        inline auto empty() const -> bool;
        auto begin() const -> iterator;
        auto end() const -> iterator;
        auto front() const -> const_reference;
        auto back() const -> const_reference;

        void clear();
        void shrink_to_fit();
        void push_back(const_reference v);
        void emplace_back(value_type&& v);
        void pop_back();
        void push_front(const_reference v);
        void emplace_front(value_type&& v);
        void pop_front();
    private:
        auto allocate_map(size_type desired_size) -> map_pointer;
        void deallocate_map();
        void copy_map_into(map_pointer other_map, size_t other_map_size);
        void reverse_copy_map_into(map_pointer other_map, size_t other_map_size);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        auto allocate_node() -> pointer;
        void push_back_auxiliary(const_reference v);
        void destroy_range(iterator a, iterator b);

        map_pointer map;
        size_type map_size;
        iterator start;
        iterator finish;
    };

    template<typename T, size_t deque_chunk_size>
    deque<T, deque_chunk_size>::deque()
     : map{nullptr}, map_size{}, start{nullptr}, finish{nullptr}
    {
        auto node0 = allocate_node();
        map = allocate_map(1);
        map[0] = node0;
        map_size = 1;
        start = iterator{map};
        finish = iterator{map};
    }

    template<typename T, size_t deque_chunk_size>
    deque<T, deque_chunk_size>::~deque() {
        destroy_range(start, finish);
        deallocate_map();
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
    auto deque<T, deque_chunk_size>::begin() const -> deque<T, deque_chunk_size>::iterator {
        return start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::end() const -> deque<T, deque_chunk_size>::iterator {
        return finish;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::front() const -> const_reference {
        return *start;
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::back() const -> const_reference {
        // Can't use *finish, and operator-- changes the iterator, so we need a cpy
        // also can't use operator-(int) since that dereferences finish
        auto tmp = finish;
        return *(--tmp);
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::clear() {
        if(empty())
            return;

        if(map_size > 2) { // Destroy all "middle" nodes
            for(auto i = 1; i < map_size-1; i++)
                destroy_range(*map[i], (*map[i]) + deque_chunk_size);
        }

        if(start.node != finish.node) { // Destroy "end" nodes
            destroy_range(start.current, start.last);
            destroy_range(finish.first, finish.current);
        } else
            destroy_range(start.current, finish.current);
        finish = start;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::push_back(const_reference v) {
        if(finish.current != finish.last - 1) {
            new(finish.current)value_type(v);
            ++finish;
        } else
            push_back_auxiliary(v);
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::destroy_range(deque<T, deque_chunk_size>::iterator a, deque<T, deque_chunk_size>::iterator b) {
        for(auto p = a; p != b; ++p)
            p.current->~T();
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::allocate_node() -> deque::pointer {
        return (pointer)malloc(deque_chunk_size * sizeof(value_type));
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::push_back_auxiliary(const_reference v) {
        reallocate_map(1, false);
        *(finish.node + 1) = allocate_node();
        finish.set_node(finish.node + 1);
        finish.current = finish.first;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::reallocate_map(deque<T, deque_chunk_size>::size_type nodes_to_add, bool add_at_front) {
        auto old_max_index = map_size - 1;
        auto new_map_size = map_size + nodes_to_add;
        auto new_map = allocate_map(new_map_size);
        if(add_at_front)
            reverse_copy_map_into(new_map, new_map_size);
        else
            copy_map_into(new_map, new_map_size);
        deallocate_map();
        map = new_map;
        map_size = new_map_size;

        map_pointer new_node_start;
        if(add_at_front)
            new_node_start = &new_map[(new_map_size - 1) - old_max_index];
        else
            new_node_start = &new_map[0];

        start.set_node(new_node_start);
        finish.set_node(new_node_start + old_max_index);
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::allocate_map(deque<T, deque_chunk_size>::size_type desired_size) -> deque<T, deque_chunk_size>::map_pointer {
        return (map_pointer)malloc(desired_size * sizeof(pointer));
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::deallocate_map() {
        free(map);
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::copy_map_into(deque<T, deque_chunk_size>::map_pointer other_map, size_t other_map_size) {
        for(auto i = 0; i < stl::min(map_size, other_map_size); i++)
            other_map[i] = map[i];
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::reverse_copy_map_into(deque<T, deque_chunk_size>::map_pointer other_map, size_t other_map_size) {
        auto other_map_max_index = other_map_size - 1;
        auto map_max_index = map_size - 1;
        for(auto i = 0; i < stl::min(map_size, other_map_size); i++)
            other_map[other_map_max_index - i] = map[map_max_index - i];
    }
}

#endif //AVRCPP_DEQUE_H
