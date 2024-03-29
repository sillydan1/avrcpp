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
/*
 * Implementation file for avr-g++ extensions
 * */
#include "utillities.h"
#include <stdlib.h>

// virtuality extensions
void __cxa_pure_virtual(void) {
	abort();
}
void __cxa_deleted_virtual(void) {
	abort();
}

// new/delete allocators
auto operator new(size_t objsize) -> void* {
	return malloc(objsize);
}
auto operator new(size_t objsize, void* ptr) -> void* {
    return ptr;
}
auto operator new[](size_t objsize) -> void* {
	return malloc(objsize);
}
auto operator new[](size_t objsize, void* ptr) -> void* {
    return ptr;
}
void operator delete(void* obj) {
	free(obj);
}
void operator delete(void* obj, size_t size) {
    free(obj);
}
void operator delete[](void* obj) {
	free(obj);
}
void operator delete[](void* obj, size_t size) {
    free(obj);
}
