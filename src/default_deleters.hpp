#ifndef DEFAULT_DELETERS_HPP
#define DEFAULT_DELETERS_HPP
#include <type_traits>

namespace std {
	//// Default deleter Functor objects. 
	//// Custom deleter support is needed for more complex types pointed to
	//// by smart pointers.
	template<typename T, typename SFINAE = void>
	struct default_deleter {
		inline void operator()(T* a) const { delete a; }
	};

	template<typename T>
	struct default_deleter<T, typename std::enable_if<std::is_array<T>::value>::type > {
		inline void operator()(typename std::remove_array<T>::value_type * a) const { delete[] a; }
	};
}

#endif // DEFAULT_DELETERS_HPP
