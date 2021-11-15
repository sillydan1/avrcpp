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
#include "default_includes"

namespace stl {
    // Integral constant, based on the "possible implementation" from cppreference
    template<typename T, T v>
    struct integral_constant {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type; // using injected-class-name
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; } //since c++14
    };
    struct false_type : integral_constant<bool, false> { };
    struct true_type : integral_constant<bool, true> { };
    template< class... >
    using void_t = void;

    // is_array, these are also based on the "possible implementation" from cppreference
    template<typename T> struct is_array : false_type {};
    template<typename T> struct is_array<T[]> : true_type { };
    template<typename T, unsigned int N> struct is_array<T[N]> : true_type {};
    // is_same, use this to check two types.
    template<typename T, typename U> struct is_same : false_type {};
    template<typename T> struct is_same<T, T> : true_type { };

    template<typename T> struct remove_array {
        typedef T value_type;
    };
    template<typename T> struct remove_array<T[]> {
        typedef T value_type;
    };
    template<typename T, size_t N> struct remove_array<T[N]> {
        typedef T value_type;
    };

    template<typename T>
    using remove_array_t = typename remove_array<T>::value_type;

    template<bool B, typename T = void>
    struct enable_if {
    };

    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    template<bool B, typename T = void>
    using enable_if_t = typename enable_if<B,T>::type;

    template<typename T > struct remove_reference      {typedef T type;};
    template<typename T > struct remove_reference<T&>  {typedef T type;};
    template<typename T > struct remove_reference<T&&> {typedef T type;};

    template<bool B, typename T, typename F> struct conditional { typedef T type; };
    template<typename T, typename F> struct conditional<false, T, F> { typedef F type; };
    template<bool B, typename T, typename F> using conditional_t = typename conditional<B,T,F>::type;

    // https://en.cppreference.com/w/cpp/types/add_reference
    namespace detail {
        template<typename T> struct type_identity { using type = T; };

        template <typename T> auto try_add_lvalue_reference(int) -> type_identity<T&>;
        template <typename T> auto try_add_lvalue_reference(...) -> type_identity<T>;

        template<typename T> auto try_add_rvalue_reference(int) -> type_identity<T&&>;
        template<typename T> auto try_add_rvalue_reference(...) -> type_identity<T>;

        template<class>
        constexpr bool is_unbounded_array_v = false;
        template<class T>
        constexpr bool is_unbounded_array_v<T[]> = true;

        template<class>
        constexpr bool is_bounded_array_v = false;
        template<class T, size_t N>
        constexpr bool is_bounded_array_v<T[N]> = true;
    }
    template<typename T> struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};
    template<typename T> struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};

    // declval
    /// Yes. There's no implementation.
    template<typename T>
    typename stl::add_rvalue_reference<T>::type declval() noexcept;

    namespace detail {
        template <typename T> char test(int T::*);
        struct two { char c[2]; };
        template <typename T> two test(...);
    }

    template <typename T>
    struct is_class : stl::integral_constant<bool, sizeof(detail::test<T>(0))==1
    > {}; // && !stl::is_union<T>::value

    // https://en.cppreference.com/w/cpp/types/is_base_of
    namespace details {
        template <typename Base> stl::true_type is_base_of_test_func(const volatile Base*);
        template <typename Base> stl::false_type is_base_of_test_func(const volatile void*);
        template <typename Base, typename Derived>
        using pre_is_base_of = decltype(is_base_of_test_func<Base>(stl::declval<Derived*>()));

        template <typename Base, typename Derived, typename = void>
        struct pre_is_base_of2 : public stl::true_type { };
        // note stl::void_t is a C++17 feature
        template <typename Base, typename Derived>
        struct pre_is_base_of2<Base, Derived, stl::void_t<pre_is_base_of<Base, Derived>>> :
                public pre_is_base_of<Base, Derived> { };
    }

    template <typename Base, typename Derived>
    struct is_base_of :
            public stl::conditional_t<
                    stl::is_class<Base>::value && stl::is_class<Derived>::value,
                    details::pre_is_base_of2<Base, Derived>,
                    stl::false_type
            > { };

    template<class T> struct is_lvalue_reference     : stl::false_type {};
    template<class T> struct is_lvalue_reference<T&> : stl::true_type {};

    template<typename T>
    bool disjunction(T compareVal, T arg0) {
        return compareVal == arg0;
    }
    template<typename T, typename... Args>
    bool disjunction(T compareVal, T arg0, Args... args) {
        if(compareVal == arg0)
            return true;
        return disjunction(compareVal, args...);
    }
    template<typename T>
    bool conjunction(T compareVal, T arg0) {
        return compareVal == arg0;
    }
    template<typename T, typename... Args>
    bool conjunction(T compareVal, T arg0, Args... args) {
        if(compareVal != arg0)
            return false;
        return conjunction(compareVal, args...);
    }
}

#endif // TYPE_TRAITS_H
