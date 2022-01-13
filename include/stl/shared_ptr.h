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
#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP
#include "default_deleters.h"
#include "default_includes"

namespace stl {
    template <typename T, typename D = default_deleter<T>>
    class shared_ptr {
        using T_t = stl::remove_array_t<T>;
        template<typename R>
        using is_derived = stl::conditional_t<
                stl::is_class<stl::remove_array_t<R>>::value,
                stl::is_base_of<T_t,stl::remove_array_t<R>>,
                stl::is_same<stl::remove_array_t<T>,stl::remove_array_t<R>>
        >;
    public:
        shared_ptr()
                : resource(nullptr), refCount(new uint8_t(1)) {}
        shared_ptr(T_t* a)
                : resource(a), refCount(new uint8_t(1))
        { }
        template<typename R>
        shared_ptr(const shared_ptr<R>& ptr2)
                : resource(static_cast<T_t*>(ptr2.get())), refCount(ptr2.get_counter())
        {
            static_assert(is_derived<R>::value, "Can only copy pointers of derived classes!");
            ++*refCount;
        }
        shared_ptr(const shared_ptr<T>& ptr2)
                : resource(ptr2.resource), refCount(ptr2.refCount)
        { ++*refCount; }
        ~shared_ptr() {
            if (refCount != NULL && --*refCount == 0)
                freeResource();
        }

        inline T_t* operator->() const {
            static_assert(!stl::is_array<T>::value,
                          "'->' operator does not work for pointers to arrays!");
            return resource;
        }
        inline T_t& operator*() const  {
            static_assert(!stl::is_array<T>::value,
                          "'*' operator does not work for pointers to arrays!");
            return *resource;
        }
        inline T_t& operator[] (const unsigned int i) const {
            static_assert(stl::is_array<T>::value,
                          "'[]' operator only works for pointers to arrays!");
            return resource[i];
        }
        inline T_t* get() const { return resource; }
        inline uint8_t* get_counter() const { return refCount; }
        inline uint8_t use_count() const {
            if(refCount == nullptr)
                return 0;
            return *refCount;
        }
        template <typename R>
        shared_ptr& operator=(const shared_ptr<R>& ptr2) noexcept {
            static_assert(is_derived<R>::value, "Can only copy pointers of derived classes!");
            if(--*refCount == 0)
                freeResource();
            resource = static_cast<T_t*>(ptr2.get());
            refCount = ptr2.get_counter();
            ++*refCount;
            return *this;
        }
        template <typename R>
        shared_ptr& operator=(shared_ptr<R>&& ptr2) noexcept {
            static_assert(is_derived<R>::value, "Can only copy pointers of derived classes!");
            if(--*refCount == 0)
                freeResource();
            resource = static_cast<T_t*>(ptr2.get());
            refCount = ptr2.get_counter();
            ++*refCount;
            return *this;
        }
        shared_ptr& operator=(const shared_ptr<T>& ptr2) noexcept {
            if(this == &ptr2) return *this;
            if(--*refCount == 0)
                freeResource();
            resource = ptr2.get();
            refCount = ptr2.get_counter();
            ++*refCount;
            return *this;
        }

    private:
        T_t* resource;
        uint8_t* refCount;

        void freeResource() {
            if(resource != nullptr)
                D{}.free(resource); // Custom deleter instantiation
            delete refCount;
        }

    };

    template<typename T, typename... Ts>
    inline auto make_shared(Ts&&... params) -> shared_ptr<T> {
        return shared_ptr<T>(new T(stl::forward<Ts>(params)...));
    }

    template <typename T, typename... Ts>
    inline auto make_shared_array(Ts&&... p) -> stl::shared_ptr<T[]> {
        return stl::shared_ptr<T[]>(new T[sizeof...(p)] {p...});
    }
}

#endif // SHARED_PTR_HPP
