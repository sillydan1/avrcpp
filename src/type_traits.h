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

namespace std {

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

    template<typename T> struct remove_array { typedef T value_type; };
    template<typename T> struct remove_array<T[]> { typedef T value_type; };
    template<typename T, size_t N> struct remove_array<T[N]> { typedef T value_type; };
    
    template <typename T> struct is_class : std::integral_constant<bool, sizeof(detail::test<T>(0))==1> {}; // && !std::is_union<T>::value
    // Based off of: https://en.cppreference.com/w/cpp/types/is_base_of
    namespace details {
        template <typename Base> std::true_type is_base_of_test_func(const volatile Base*);
        template <typename Base> std::false_type is_base_of_test_func(const volatile void*);
        template <typename Base, typename Derived>
        using pre_is_base_of = decltype(is_base_of_test_func<Base>(std::declval<Derived*>()));

        template <typename Base, typename Derived, typename = void>
        struct pre_is_base_of2 : public std::true_type { };
        // note std::void_t is a C++17 feature
        template <typename Base, typename Derived>
        struct pre_is_base_of2<Base, Derived, std::void_t<pre_is_base_of<Base, Derived>>> :
                public pre_is_base_of<Base, Derived> { };
    }

    template <typename Base, typename Derived>
    struct is_base_of :
            public std::conditional_t<
                    std::is_class<Base>::value && std::is_class<Derived>::value,
                    details::pre_is_base_of2<Base, Derived>,
                    std::false_type
            > { };
}
#endif // TYPE_TRAITS_H
