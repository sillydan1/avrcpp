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
#ifndef AVRCPP_ALGORITHM_H
#define AVRCPP_ALGORITHM_H

namespace stl {
    template<class T>
    const T& max(const T& a, const T& b) {
        return (a < b) ? b : a;
    }
    template<class T, class Compare>
    const T& max(const T& a, const T& b, Compare comp) {
        return (comp(a, b)) ? b : a;
    }
    template<class T>
    const T& min(const T& a, const T& b) {
        return (a < b) ? a : b;
    }
    template<class T, class Compare>
    const T& min(const T& a, const T& b, Compare comp) {
        return (comp(a, b)) ? a : b;
    }
    // TODO: stl::max for initializer_lists
}

#endif //AVRCPP_ALGORITHM_H
