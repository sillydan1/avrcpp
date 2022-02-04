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
#ifndef UTILLITIES_H
#define UTILLITIES_H
#include <stddef.h>
/* Compiler extensions */
extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));
/* new/delete allocator functions */
auto operator new(size_t size) -> void*;
auto operator new(size_t size, void* ptr) -> void*;
auto operator new[](size_t size) -> void*;
auto operator new[](size_t size, void* ptr) -> void*;
void operator delete(void * ptr);
void operator delete(void * ptr, size_t size);
void operator delete[](void * ptr);
void operator delete[](void * ptr, size_t size);
#endif
