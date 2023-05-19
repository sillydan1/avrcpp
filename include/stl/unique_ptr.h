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
#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP
#include "default_deleters.h"
#include "../utility"

namespace stl {
    /// Unique pointer class. Pointer with unique ownership of
    /// the resource. You can define custom deleters via the second
    /// template argument.
    /// Usage:
    /// unique_ptr<int> my_int_pointer;
    template <typename T, typename D = default_deleter<T>>
    class unique_ptr {
        using T_t = stl::remove_array_t<T>;
        template<typename R>
        using is_derived = stl::conditional_t<
                stl::is_class<stl::remove_array_t<R>>::value,
                stl::is_base_of<T_t,stl::remove_array_t<R>>,
                stl::is_same<stl::remove_array_t<T>,stl::remove_array_t<R>>
        >;
    public:
        unique_ptr() = delete;
        unique_ptr(const unique_ptr& other) = delete;
        template<typename R>
        unique_ptr(unique_ptr<R>&& ptr2)
                : resource(static_cast<T_t*>(ptr2.get()))
        { static_assert(is_derived<R>::value, "Can only move pointers of derived classes!"); }
        unique_ptr(T_t* a) : resource(a) {}
        ~unique_ptr() {
            if(resource != NULL)
                freeResource();
        }
        unique_ptr<T,D>& operator=(const unique_ptr<T, D> &ptr2) = delete;

        inline T* operator->() const {
            static_assert(!stl::is_array<T>::value,
                          "'->' operator does not work for pointers to arrays!");
            return resource;
        }
        inline T& operator*() const {
            static_assert(!stl::is_array<T>::value,
                          "'*' operator does not work for pointers to arrays!");
            return *resource;
        }
        inline T_t& operator[](const unsigned int i) const {
            static_assert(stl::is_array<T>::value,
                          "'[]' operator only works for pointers to arrays!");
            return resource[i];
        }
        inline T_t* const get() const { return resource; }
        template<typename R>
        unique_ptr& operator=(unique_ptr<R>&& ptr) noexcept {
            static_assert(is_derived<R>::value, "Can only move pointers of derived classes!");
            if (resource != NULL)
                freeResource();
            resource = ptr.resource; /* "move" ownership of the resource to us */
            ptr.resource = NULL;
            return *this;
        }

    private:
        T_t* resource;
        void freeResource() {
            D{}.free(resource);
        }
    };

    template<class T, class... Args>
    auto make_unique(Args&&... args) -> stl::enable_if_t<!stl::is_array<T>::value, stl::unique_ptr<T>> {
        return stl::unique_ptr<T>(new T(stl::forward<Args>(args)...));
    }

    template<class T>
    auto make_unique(size_t n) -> stl::enable_if_t<detail::is_unbounded_array_v<T>, stl::unique_ptr<T>> {
        return stl::unique_ptr<T>(new stl::remove_array<T>[n]());
    }

    template<class T, class... Args>
    auto make_unique(Args&&...) -> stl::enable_if_t<detail::is_bounded_array_v<T>> = delete;
}

#endif // UNIQUE_PTR_HPP
