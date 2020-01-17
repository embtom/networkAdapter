/* -----------------------------------------------------------------------------------------------

meta::registerMembers<T> is used for class registration and it has the following form when specialized:

namespace meta
{

template <>
auto registerMembers<YourClass>()
{
    return members(
        member(...),
        ...
    );
}

}

! Some important details:
1) This specialization should be defined in header, because compiler needs to deduce the return type.
2) This function is called by MetaHolder during it's static member initialization, so the tuple is created
   only once and then registerMembers function is never called again.
3) registerMembers could easily be a free function, but befriending such function is hard if you want to
   be able to get pointers to private members... Writing "friend class Meta" in your preferred class
   is just much easier. Though this might be somehow fixed in the future.
4) If the class is not registered then doForAllMembers(<your function>) will do nothing,
   because the function will return empty tuple.
5) MemberPtr.h is included in this file just so that user can #include "Meta.h" and get MemberPtr.h
   included too, which is always needed for registration.

-------------------------------------------------------------------------------------------------*/

#ifndef _ENDIANMETA_H_
#define _ENDIANMETA_H_

#include <type_traits>
#include <tuple>
#include <utility>
#include <EndianMembers.h>

namespace EtEndian
{
    
/*********************************************
 * User class registration
 * *******************************************/

// template for class name registration
// and have to be specialized by the user
template <typename Class>
constexpr auto registerName();

// template for class members registrations
// and have to be specialized by the user
template <typename Class>
inline auto registerMembers();

// helper routine to package the list of members into a tuple 
template <typename... Args>
auto members(Args&&... args);

/*********************************************
 * Interfacing to data members of registerd 
 * class or struct 
 * *******************************************/

// Request of Class Name, registerd
// by template specialization of "registerName"
template <typename Class>
constexpr auto getName();

// Request the number data members, registerd
// by template specialization of "registerMembers"
template <typename Class>
constexpr std::size_t getMemberCount();

// Requests the array of data members, registerd
// by template specialization of "registerMembers"
template <typename Class>
const auto& getMembers();

//Check if Class is correct registerd
template <typename Class>
constexpr bool isRegistered();

/*********************************************
 * Iterate through the list registerd data members
 * *******************************************/
template <typename Class, typename F,
          typename = std::enable_if_t<isRegistered<Class>()>>
void doForAllMembers(F&& f);

// version for non-registered classes (to generate less template stuff)
template <typename Class, typename F,
          typename = std::enable_if_t<!isRegistered<Class>()>,
          typename = void>
void doForAllMembers(F&& f);

} //EtEndian

#include "EndianMeta.inl"

#endif /*_ENDIANMETA_H_*/