#ifndef _ENDIANMEMBERS_H_
#define _ENDIANMEMBERS_H_

#include <type_traits>

namespace EtEndian
{

template <typename Class, typename T>
using member_ptr_t = T Class::*;

template <typename MemberType>
using get_member_type = typename std::decay_t<MemberType>::member_type;

template <typename Class, typename T>
class Member
{
public:
    using class_type = Class;
    using member_type = std::enable_if_t<!std::is_array<T>::value, T>;

    Member(const char* name, member_ptr_t<Class, T> ptr) :
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

    member_type getCopy(const Class& obj) const
    { return obj.*m_ptr; }

    member_type& getRef(Class& obj) const
    { return obj.*m_ptr; }

    const member_type& getConstRef(const Class& obj) const
    { return obj.*m_ptr; }

    member_ptr_t<Class, T> getPtr() const
    { return *m_ptr; }


private:
    const char*            m_name;
    member_ptr_t<Class, T> m_ptr;
};


// useful function similar to make_pair which is used so you don't have to write this:
// Member<SomeClass, int>("someName", &SomeClass::someInt); and can just to this:
// member("someName", &SomeClass::someInt);
template <typename Class, typename T>
Member<Class, T> member(const char* name, T Class::* ptr)
{
    return Member<Class, T>(name, ptr);
}

} // end of namespace

#endif