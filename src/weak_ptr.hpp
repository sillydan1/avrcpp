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
#ifndef WEAK_PTR_HPP
#define WEAK_PTR_HPP
#include <utillities.h>

namespace std {
	
	template<typename T>
	class weak_ptr {
	public:
		weak_ptr(const weak_ptr<T>& other) : resource{other.resource}, count{other.count}
		{ release_resource(); (*count)++; }
		~weak_ptr() {
			release_resource();
		}
		
		void move(weak_ptr<T>& other) {
			// Simply move the resource. Since we're moving, we shouldn't count up
		}
		
	private:
		T* resource;
		unsigned int* count;
		
		void release_resource() {
			if(--*count <= 0) {
				delete resource;
				// --- ↓ --- //
				resource->~T();
				free(resource);
			}
		}
		
		weak_ptr() = delete;
	}

}

#define MAKEWEAK(type, ...) std::weak_ptr<type>(new type(__VA_ARGS__))

#endif //WEAK_PTR_HPP
