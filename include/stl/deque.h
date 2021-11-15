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
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        auto allocate_node() -> pointer;
        void push_back_auxiliary(const_reference v);
        void pop_back_auxiliary(const_reference v);
        void push_front_auxiliary(const_reference v);
        void pop_front_auxiliary(const_reference v);
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
        auto node0 = (pointer)malloc(deque_chunk_size * sizeof(value_type));
        map = allocate_map(1);
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
        return *(finish-1);
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::clear() {
        if(empty())
            return;

        if(map_size > 2) { // Destroy all "middle" nodes
            for (map_pointer node = start.node + 1; node < finish.node; ++node)
                destroy_range(*node, (*node) + deque_chunk_size);
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
            // construct
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
    void deque<T, deque_chunk_size>::destroy_range(deque<T, deque_chunk_size>::pointer a, deque<T, deque_chunk_size>::pointer b) {
        for(auto p = a; p != b; ++p)
            p->~T();
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::allocate_node() -> deque::pointer {

        return nullptr;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::push_back_auxiliary(const_reference v) {
        auto v_cpy = v;
        // reserve map at back
        *(finish.node+1) = allocate_node();
        finish.set_node(finish.node + 1);
        finish.current = finish.first;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::reallocate_map(deque<T, deque_chunk_size>::size_type nodes_to_add, bool add_at_front) {
        auto old_num_of_nodes = map_size;
        auto new_num_of_nodes = old_num_of_nodes + nodes_to_add;
        map_pointer new_node_start;
        if(map_size > 2 * new_num_of_nodes) {

        } else {
            auto new_map_size = map_size + stl::max(map_size, nodes_to_add) + 2;
            auto new_map = allocate_map(new_map_size);
            new_node_start = new_map + (new_map_size - new_num_of_nodes) / 2
                             + (add_at_front ? nodes_to_add : 0);
            // copy
            deallocate_map();
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_node_start);
        finish.set_node(new_node_start + old_num_of_nodes - 1);
    }

    template<typename T, size_t deque_chunk_size>
    auto deque<T, deque_chunk_size>::allocate_map(deque<T, deque_chunk_size>::size_type desired_size) -> deque<T, deque_chunk_size>::map_pointer {
        return (map_pointer)malloc(desired_size * sizeof(pointer));;
    }

    template<typename T, size_t deque_chunk_size>
    void deque<T, deque_chunk_size>::deallocate_map() {

    }
}

#endif //AVRCPP_DEQUE_H
