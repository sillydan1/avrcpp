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
