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
#include <utillities.h>
#include "default_deleters.hpp"

namespace std {

	template<typename T, typename D = default_deleter<T>>
	class unique_ptr {
        using Tt = std::remove_array_t<T>;
        template<typename L>
        using is_derived = std::conditional_t<
            std::is_class<std::remove_array_t<L>>::value,
            std::is_base_of<Tt,std::remove_array_t<L>>,
            std::is_same<Tt,std::remove_array_t<L>>
        >;
	public:
        unique_ptr(const unique_ptr&) = delete;
        unique_ptr() = delete;
        template<typename L>
        unique_ptr(unique_ptr<L>&& ptr)
         : resource(static_cast<Tt*>(ptr.get()))
        { static_assert(is_derived<L>::value, "Only unique pointers of derived classes can be moved"); }
        unique_ptr(Tt* ptr) : resource{ptr} {}
        ~unique_ptr() { release_resource(); }
        // No implicit copy assignment, please
        unique_ptr<T,D>& operator=(const unique_ptr<T,D>& ptr) = delete;
		
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
        template<typename L>
        unique_ptr& operator=(unique_ptr<L>&& p) {
            static_assert(is_derived<L>::value, "Can only move pointers of derived classes!");
            if (resource != NULL)
                release_resource();
            resource = p.resource; // move ownership of the resource to this
            p.resource = NULL;
            return *this;
        }
		
	private:
		Tt* resource;
		void release_resource() { D::free(resource); }
	};

}

#define MAKEUNIQUE(type, ...) std::unique_ptr<type>(new type(__VA_ARGS__))

#endif // UNIQUE_PTR_HPP
