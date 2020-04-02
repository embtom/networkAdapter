#ifndef _TEMPLATE_HELPERS_H_
#define _TEMPLATE_HELPERS_H_

namespace EtEndian {
namespace detail {

// for_each_arg - call f for each element from tuple
template <typename F, typename... Args>
void for_tuple(F&& f, const std::tuple<Args...>& tuple);

// overload for empty tuple which does nothing
template <typename F>
void for_tuple(F&& f, const std::tuple<>& tuple);

//to check if pass TYPE is a is std::array
template<class T>
struct is_array :  std::false_type{};

template<class T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};

//type trait to arry count of passed c-style type
template<typename T, typename Enable = void>
struct array_count
{
    static constexpr int value = 1;
};

template<typename T>
struct array_count<T, typename std::enable_if<std::is_array<T>::value>::type>
{
    static constexpr int value = sizeof(T) / sizeof(typename std::remove_pointer_t<typename std::decay_t<T>>);
};

template <typename T>
  inline constexpr int array_count_v = array_count<T>::value;


//remove cont, volotile, and reference from passed type
template< class T >
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;


} // end of namespace detail
} // end of namespace meta

#include "template_helpers.inl"

#endif