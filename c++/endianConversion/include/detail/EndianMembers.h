#ifndef _ENDIANMEMBERS_H_
#define _ENDIANMEMBERS_H_

#include <cstring>
#include <type_traits>
#include <detail/template_helpers.h>


namespace EtEndian
{

//template <typename Class, typename T>

template <typename MemberType>
using get_member_type = typename std::decay_t<MemberType>::member_type;


template <typename Class, typename T>
class Member
{ };

template <typename Class, typename T>
class Member<Class, T Class::*>
{
public:
    using member_ptr_t = T Class::*;
    using class_type = Class;
    using member_type = std::enable_if_t<!std::is_array<T>::value, T>;

    Member(const char* name, member_ptr_t ptr) :
        m_name(name),
        m_ptr(ptr)
    {}

    const char* getName() const
    { return m_name; }

    const member_type& get(const Class& obj) const
    { return obj.*m_ptr; };

    template <typename V,
              typename std::enable_if_t<std::is_constructible_v<T, V>, int> = 0>
    void set(Class& obj, V&& value) const
    { obj.*m_ptr = value; }

    member_type& getRef(Class& obj) const
    { return obj.*m_ptr; }

    const member_type& getConstRef(const Class& obj) const
    { return obj.*m_ptr; }

    member_ptr_t getPtr() const
    { return *m_ptr; }

private:
    const char*            m_name;
    member_ptr_t m_ptr;
};


template <typename Class, typename T, int N>
class Member<Class, T (Class::*)[N]>
{
public:
    using class_type = Class;
    using member_ptr_t = T (Class::*)[N];
    using member_type = std::enable_if_t<std::is_array<T[N]>::value, T[N]>;

    Member(const char* name, member_ptr_t ptr) :
        m_name(name),
        m_ptr(ptr)
    { }

    const char* getName() const
    {   return m_name; }

    const member_type& get(const Class& obj) const
    {   return obj.*m_ptr; };

    //the remove_cvref_t(std::remove_reference_t) is necessary, to remove the reference type of the array, passed as universal reference.
    // for e.g (is_array<V> is false because it is reference to a array),
    //         (is_array<remove_cvref_t<V>> is true because the reference type of the array is removed)
    template <typename V, typename std::enable_if_t<std::is_same_v<detail::remove_cvref_t<V>, member_type>, int> = 0>
    void set(Class& obj, V&& value) const
    {
        constexpr int size = detail::array_count<detail::remove_cvref_t<V>>::value;
        std::memcpy(obj.*m_ptr, &value, size);
    }

    member_type& getRef(Class& obj) const
    { return obj.*m_ptr; }

    const member_type& getConstRef(const Class& obj) const
    { return obj.*m_ptr; }

    member_ptr_t getPtr() const
    { return *m_ptr; }

private:
    const char*            m_name;
    member_ptr_t           m_ptr;
};


// useful function similar to make_pair which is used so you don't have to write this:
// Member<SomeClass, int>("someName", &SomeClass::someInt); and can just to this:
// member("someName", &SomeClass::someInt);
template <typename Class, typename T>
auto member(const char* name, T Class::* ptr)
{
    return Member<Class, decltype(ptr)>(name, ptr);
}

template<typename Class, typename T, int N>
auto member(const char *name, T (Class::* ptr)[N])
{
    return Member<Class, decltype(ptr)> (name, ptr);
}

} // end of namespace

#endif