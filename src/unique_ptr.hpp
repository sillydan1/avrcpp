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

namespace std {

	template<typename T>
	class unique_ptr {
	public:
		explicit unique_ptr(T* res) : resource{res} 
		{ }
		~unique_ptr() { release_resource(); }
		
		inline T* const operator->() const { return resource; }
		inline T& operator*() const { return *resource; }
		
		void move(unique_ptr<T>& other) { // Move 'this' into 'other'
			other.release_resource();
			other.resource = resource;
			resource = NULL;
		}
		
		inline T* const get() const { return resource; }
		
	private:
		T* resource;
		// Disable default construction and copying
		unique_ptr() = delete;
		unique_ptr(const unique_ptr<T>&) = delete;

		void release_resource() {
			delete resource;
			// --- ↓ --- //
			resource->~T();
			free(resource);
		}
	};

}

#define MAKEUNIQUE(type, ...) std::unique_ptr<type>(new type(__VA_ARGS__))

#endif // UNIQUE_PTR_HPP
