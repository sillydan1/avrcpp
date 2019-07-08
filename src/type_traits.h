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
#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant type;
};

struct true_type  : integral_constant<bool, true> { }

struct false_type : integral_constant<bool, false>{ }

template<bool B, typename T, typename F>
struct conditional { typedef T type; };
 
template<typename T, typename F>
struct conditional<false, T, F> { typedef F type; };

template<bool B, typename T = void>
struct enable_if {};
 
template<typename T>
struct enable_if<true, T> { typedef T type; };

#endif // TYPE_TRAITS_H
