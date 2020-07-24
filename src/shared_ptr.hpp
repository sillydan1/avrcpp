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
#include <utillities.h>
#include "default_deleters.hpp"
#include <stdio.h>
#include <stdlib.h>

namespace std {
    /// The type-range of maximum amount of shares between shared_ptr
    /// just change this to unsigned int, if it makes you feel better
    /// - also, please don't use anything signed. That would be moronic.
#ifndef SHARED_PTR_COUNTER_TYPE
#define SHARED_PTR_COUNTER_TYPE unsigned char
#endif
    using shcnt_t = SHARED_PTR_COUNTER_TYPE; 
    
	template<typename T, typename D = default_deleter<T>>
	class shared_ptr {
        using Tt = std::remove_array_t<T>;
        template<typename L>
        using is_derived = std::conditional_t<
                std::is_class<std::remove_array_t<L>>::value,
                std::is_base_of<Tt,std::remove_array_t<L>>,
                std::is_same<Tt,std::remove_array_t<L>>
        >;
	public:
        shared_ptr() = delete;
		shared_ptr(Tt* ptr) : resource{ptr}, count{new shcnt_t(1)}
		{ }
        template<typename L>
        shared_ptr(const shared_ptr<R>& p)
         : resource{static_cast<Tt*>(p.get())}, count{p.count}
		{
            static_assert(is_derived<R>::value,
            "Can only copy pointers of derived classes");
            ++*count;
        }
        shared_ptr(const shared_ptr<T>& p)
         : resource{p.resource}, count(p.count)
		{ ++*count; }
        ~shared_ptr() {
            release_resource();
        }
        
        inline Tt& operator[](const unsigned char i) const {
            static_assert(std::is_array<T>::value,
            "'[]' operator only work on pointers to arrays");
        }
        inline T* operator->() const {
            static_assert(!std::is_array<T>::value,
            "'->' operator doesn't work on pointers to arrays");
            return resource;
        }
        inline T& operator*() const {
            static_assert(!std::is_array<T>::value,
            "'*' operator doesn't work on pointers to arrays");
            return *resource;
        }
        inline Tt* const get() const { return resource; }
        inline shcnt_t use_count() const { 
            if(count == NULL) return 0; else return *count; 
        }
        template<typename L>
        shared_ptr& operator=(const shared_ptr<L>& p) {
            static_assert(is_derived<L>::value, 
            "Can only copy pointers of derived classes!");
            release_resource();
            resource = static_cast<Tt*>(p.resource);
            count = p.count;
            ++*count;
            return *this;
        }
        shared_ptr& operator=(const shared_ptr<T>& p) { // Copying sh<T> and not sh<L>
            static_assert(is_derived<T>::value, 
            "Can only copy pointers of derived classes!");
            release_resource();
            resource = static_cast<Tt*>(p.resource);
            count = p.count;
            ++*count;
            return *this;
        }
        template<typename L>
        shared_ptr& operator=(shared_ptr<L>&& p) {
            static_assert(is_derived<L>::value, 
            "Can only move pointers of derived classes!");
            release_resource();
            resource = static_cast<Tt*>(p.resource);
            count = p.count;
            ++*count;
            return *this;
        }
        
	private:
		T* resource;
		shcnt_t* count;
		
		void release_resource() {
			if(--*count <= 0) {
				D::free(resource);
                delete count;
			}
		}
		
		shared_ptr() = delete;
	};

    // Having make_shared for arrays is a C++20 feature
    template<typename T, typename... Ts>
    inline shared_ptr<T> make_shared(Ts&&... ps) {
        return shared_ptr<T>(new T(std::forward<Ts>(ps)...));
    }

    template <typename T, typename... Ts>
    inline std::shared_ptr<T[]> make_shared_array(Ts&&... p) {
        return std::shared_ptr<T[]>(new T[sizeof...(p)] {p...});
    }
	
}

#endif // SHARED_PTR_HPP
