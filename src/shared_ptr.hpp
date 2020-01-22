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
    using shcnt_t = unsigned char; 
    
	template<typename T, typename D = default_deleter<T>>
	class shared_ptr {
        using T_t = std::remove_array_t<T>;
        template<typename R>
        using is_derived = std::conditional_t<
                std::is_class<std::remove_array_t<R>>::value,
                std::is_base_of<T_t,std::remove_array_t<R>>,
                std::is_same<std::remove_array_t<T>,std::remove_array_t<R>>
        >;
	public:
        shared_ptr() = delete;
		shared_ptr(T* res) : resource{res}, count{new shcnt_t(1)}
		{ }
		shared_ptr(const shared_ptr<T>& other) : resource{other.resource}, count{other.count}
		{ release_resource(); (*count)++; }
		~shared_ptr() {
			release_resource();
		}
		
		void move(shared_ptr<T>& other) {
			// Simply move the resource. Since we're moving, we shouldn't count up
		}
		
	private:
		T* resource;
		unsigned int* count; // TODO: Support weak pointers
		
		void release_resource() {
			if(--*count <= 0) {
				delete resource;
				// --- ↓ --- //
				resource->~T();
				free(resource);
			}
		}
		
		shared_ptr() = delete;
	};
	
}

#define MAKESHARED(type, ...) std::shared_ptr<type>(new type(__VA_ARGS__))

#endif // SHARED_PTR_HPP
