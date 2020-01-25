// Various useful template stuff
// thanks to Vittorio Romeo and other people who came up with this stuff

#pragma once

namespace EtEndian {
namespace detail {

// for_each_arg - call f for each element from tuple
template <typename F, typename... Args>
void for_tuple(F&& f, const std::tuple<Args...>& tuple);

// overload for empty tuple which does nothing
template <typename F>
void for_tuple(F&& f, const std::tuple<>& tuple);

//type trait to check if is std::array
template<class T>
struct is_array :  std::false_type{};

template<class T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};

} // end of namespace detail
} // end of namespace meta

#include "template_helpers.inl"
