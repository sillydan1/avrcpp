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
namespace stl {
#ifdef AVRCPP_DEQUE_H
    template<typename T>
    struct _deque_iterator {
        using value_type = T;
        using pointer = value_type*;
        using map_pointer = value_type**;
        using reference = value_type&;
        using size_type = size_t;
        using self_type = _deque_iterator<value_type>;
        using difference_type = ptrdiff_t;

        void set_node(map_pointer new_node) {
            auto block_size = chunk_size();
            node = new_node;
            first = *new_node;
            last = first + block_size;
        }
        // This could probably be optimized away with a constexpr
        auto chunk_size() -> size_type {
            return last - first;
        }
        auto operator*() const -> reference {
            return *current;
        }
        auto operator->() const -> pointer {
            return current;
        }
        auto operator++() -> self_type& {
            ++current;
            if(current != last)
                return *this;
            set_node(node+1);
            current = first;
            return *this;
        }
        auto operator++(int) -> self_type {
            auto tmp = *this;
            ++*this;
            return tmp;
        }
        auto operator--() -> self_type& {
            if(current == first) {
                set_node(node - 1);
                current = last;
            }
            --current;
            return *this;
        }
        auto operator--(int) -> self_type {
            auto tmp = *this;
            --*this;
            return tmp;
        }
        auto operator+=(difference_type n) -> self_type& {
            auto offset = n + (current - first);
            if (offset >= 0 && offset < difference_type(chunk_size()))
                current += n;
            else {
                auto node_offset = offset > 0 ?
                        offset / difference_type(chunk_size())
                        : -((-offset - 1) / difference_type(chunk_size())) - 1;
                set_node(node + node_offset);
                current = first + (offset - node_offset * chunk_size());
            }
            return *this;
        }
        auto operator+(difference_type n) const -> self_type& {
            auto tmp = *this;
            return tmp += n;
        }
        auto operator-=(difference_type n) -> self_type& {
            return *this -= -n;
        }
        auto operator-(difference_type n) const -> self_type& {
            auto tmp = *this;
            return tmp -= n;
        }
        pointer current;
        pointer first;
        pointer last;
        map_pointer node;
    };
#endif

}
