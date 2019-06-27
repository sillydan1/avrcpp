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

template<typename T>
class shared_ptr {
public:
	shared_ptr(T* res) : resource{res}, count{new unsigned int(1)}
	{ }
	shared_ptr(const shared_ptr<T>& other) : resource{other.resource}, count{other.count}
	{ (*count)++; }
	~shared_ptr() {
		
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

#endif // SHARED_PTR_HPP
