#include <cassert>
#include <tuple>
#include <cstring>

#include <EndianMembers.h>
#include <detail/template_helpers.h>
#include <detail/MetaHolder.h>

namespace EtEndian
{

/*********************************************
 * User class registration
 * *******************************************/

// template for class name registration
// and have to be specialized by the user
template <typename Class>
constexpr auto registerName()
{
    return "";
}

// template for class members registrations
// and have to be specialized by the user
template <typename Class>
inline auto registerMembers()
{
    return std::make_tuple();
}

// helper routine to package the list of members into a tuple 
template <typename... Args>
auto members(Args&&... args)
{
    return std::make_tuple(std::forward<Args>(args)...);
}

/*********************************************
 * Interfacing to data members of registerd 
 * class or struct 
 * *******************************************/

// Request of Class Name, registerd
// by template specialization of "registerName"
template <typename Class>
constexpr auto getName()
{
    return detail::MetaHolder<Class, decltype(registerMembers<Class>())>::name();
}

// Request the number data members, registerd
// by template specialization of "registerMembers"
template <typename Class>
constexpr std::size_t getMemberCount()
{
    return std::tuple_size_v<decltype(registerMembers<Class>())>;
}

// Requests the array of data members, registerd
// by template specialization of "registerMembers"
template <typename Class>
const auto& getMembers()
{
    return detail::MetaHolder<Class, decltype(registerMembers<Class>())>::members;
}

//Check if Class is correct registerd
template <typename Class>
constexpr bool isRegistered()
{
    return !std::is_same_v<std::tuple<>, decltype(registerMembers<Class>())>;
}

/*********************************************
 * Iterate through the list registerd data members
 * *******************************************/
template <typename Class, typename F, typename>
void doForAllMembers(F&& f)
{
    detail::for_tuple(std::forward<F>(f), getMembers<Class>());
}

} //EtEndian