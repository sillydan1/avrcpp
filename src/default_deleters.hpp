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
#ifndef DEFAULT_DELETERS_HPP
#define DEFAULT_DELETERS_HPP
#include <type_traits>

namespace std {
	//// Default deleter Functor objects. 
	//// Custom deleter support is needed for more complex types pointed to
	//// by smart pointers.
	template<typename T, typename SFINAE = void>
	struct default_deleter {
		static inline void free(T* a) { delete a; }
	};

	template<typename T>
	struct default_deleter<T, typename std::enable_if<std::is_array<T>::value>::type > {
		static inline void free(typename std::remove_array<T>::value_type * a) { delete[] a; }
	};
}

#endif // DEFAULT_DELETERS_HPP
