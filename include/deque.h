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
#ifndef AVRCPP_DEQUE_BUFFER_SIZE
// Note: this is the ELEMENT AMOUNT in a deque buffer - not byte size
#define AVRCPP_DEQUE_BUFFER_SIZE 10
#endif

namespace stl {
    template<typename T>
    class deque {
    public:
        using value_type = T;
        using map_pointer = value_type**;
        using reference = value_type&;
        using pointer = value_type*;
        using const_reference = const value_type&;
        using size_type = size_t;
        using iterator = _deque_iterator<value_type>;
        static constexpr auto _deque_chunk_size = AVRCPP_DEQUE_BUFFER_SIZE;

        deque();
        ~deque();

        [[nodiscard]] inline auto size() const -> size_type;
        [[nodiscard]] inline auto empty() const -> bool;
        void shrink_to_fit();

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


    template<typename T>
    deque<T>::deque()
            : map{nullptr},
              map_size{0},
              start{nullptr},
              finish{start}
    {

    }

    template<typename T>
    deque<T>::~deque() {
        destroy_range(start, finish);
        // TODO: deallocate map
    }

    template<typename T>
    auto deque<T>::size() const -> deque<T>::size_type {
        return finish - start;
    }

    template<typename T>
    auto deque<T>::empty() const -> bool {
        return finish == start;
    }

    template<typename T>
    void deque<T>::shrink_to_fit() {

    }

    template<typename T>
    auto deque<T>::begin() -> deque<T>::iterator {
        return start;
    }

    template<typename T>
    auto deque<T>::begin() const -> deque<T>::iterator {
        return start;
    }

    template<typename T>
    auto deque<T>::end() -> deque<T>::iterator {
        return finish;
    }

    template<typename T>
    auto deque<T>::end() const -> deque<T>::iterator {
        return finish;
    }

    template<typename T>
    auto deque<T>::front() -> reference {
        return *this->start;
    }

    template<typename T>
    auto deque<T>::front() const -> const_reference {
        return *this->start;
    }

    template<typename T>
    auto deque<T>::back() -> reference {
        return *(end()-1);
    }

    template<typename T>
    auto deque<T>::back() const -> const_reference {
        return *(end()-1);
    }

    template<typename T>
    void deque<T>::clear() {
        if(empty())
            return;
        // Destroy all "middle" nodes
        for(auto node = start.node+1; node < finish.node; ++node) {
            destroy_range(*node, (*node) + _deque_chunk_size);
            // TODO: deallocate node
        }
        // Destroy "end" nodes
        if(start.node != finish.node) {
            destroy_range(start.current, start.last);
            destroy_range(finish.first, finish.current);
            // TODO: deallocate finish.node (don't deallocate start.node)
        } else
            destroy_range(start.current, finish.current);
        finish = start;
    }

    template<typename T>
    void deque<T>::push_back(const_reference v) {

    }

    template<typename T>
    void deque<T>::emplace_back(value_type&& v) {

    }

    template<typename T>
    void deque<T>::pop_back() {

    }

    template<typename T>
    void deque<T>::push_front(const_reference v) {

    }

    template<typename T>
    void deque<T>::emplace_front(value_type&& v) {

    }

    template<typename T>
    void deque<T>::pop_front() {

    }

    template<typename T>
    void deque<T>::destroy_range(deque<T>::iterator a, deque<T>::iterator b) {
        for(auto p = a; p != b; ++p)
            p.current->~T();
    }

    template<typename T>
    void deque<T>::destroy_range(deque<T>::pointer a, deque<T>::pointer b) {
        for(auto p = a; p != b; ++p)
            p->~T();
    }
}

#endif //AVRCPP_DEQUE_H
