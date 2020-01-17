/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2019 Thomas Willetal 
 * (https://github.com/embtom)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _ENDIANCONVERTER_H_
#define _ENDIANCONVERTER_H_

#include <string>
#include <type_traits>
#include <array>
#include <EndianMeta.h>
#include <EndianMembers.h>

namespace EtEndian
{

//type trait to check if is std::array
template<class T>
struct is_array :  std::false_type{};

template<class T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};


template<typename T>
class ConverterFunc
{

public:
    explicit ConverterFunc(const T& rObj) noexcept : 
        m_initialObj(rObj) 
    { };

    template<typename Member,
             typename = std::enable_if_t<std::is_arithmetic<EtEndian::get_member_type<Member>>::value>
    >
    void operator()(Member& member) noexcept
    {
        using element_t = EtEndian::get_member_type<Member>;
        const element_t& source = member.getConstRef(m_initialObj);  
        member.set(m_convertedObj, host_to_network(source));
    }

    template<typename Member,
                typename = std::enable_if_t<is_array<EtEndian::get_member_type<Member> >::value>,
                typename = void
    >   
    void operator()(Member& member) noexcept
    {
        using array_t = EtEndian::get_member_type<Member>;
        using array_element_t = std::decay_t<decltype(std::declval<array_t>()[0])>;

        static_assert(std::is_arithmetic<array_element_t>::value, "No arithmetic type");
        
        constexpr std::size_t N = decltype(std::declval<array_t>()){}.size();

        const array_t& sourceArray= member.getConstRef(m_initialObj);
        array_t& destArray= member.getRef(m_convertedObj);

        std::transform(sourceArray.begin(), sourceArray.end(), destArray.begin(), 
            [](const auto& a) {return host_to_network(a);});
    }

    const T& initial() const noexcept
    {
        return m_initialObj;
    }

    const T& converted() const noexcept
    {
        return m_convertedObj;
    }

private:
    T    m_initialObj;
    T    m_convertedObj;
};


}

#endif // _ENDIANCONVERTER_H_