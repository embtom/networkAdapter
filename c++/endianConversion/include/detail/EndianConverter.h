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

//******************************************************************************
// Header

#include <string>
#include <type_traits>
#include <array>
#include <templateHelpers.h>
#include <EndianMeta.h>
#include <EndianMembers.h>
#include <EndianConvert.h>

namespace EtEndian
{

enum class EConvertMode
{
    NET_ORDER,
    HOST_ORDER
};

//*****************************************************************************
//! \brief ConverterFunc
//!
template<typename T>
class ConverterFunc
{

public:
    template<typename U>
    ConverterFunc(EConvertMode mode, U&& rObj) noexcept :
        m_ConvertMode(mode),
        m_initialObj(std::forward<U>(rObj))
    { };

    ConverterFunc(const ConverterFunc&) = default;

    template<typename Member, std::enable_if_t<std::is_arithmetic<EtEndian::get_member_type<Member>>::value, int> = 0>
    void operator()(const Member& member) noexcept
    {
        using element_t = EtEndian::get_member_type<Member>;
        const element_t& source = member.getConstRef(m_initialObj);

        switch (m_ConvertMode)
        {
            case EConvertMode::NET_ORDER:
            {
                member.set(m_convertedObj, host_to_network(source));
                break;
            }
            case EConvertMode::HOST_ORDER:
            {
                member.set(m_convertedObj, network_to_host(source));
                break;
            }
        }
    }

    template<typename Member, std::enable_if_t<utils::is_array<EtEndian::get_member_type<Member>>::value, int> = 0>
    void operator()(const Member& member) noexcept
    {
        using array_t = EtEndian::get_member_type<Member>;
        using array_element_t = std::decay_t<decltype(std::declval<array_t>()[0])>;

        static_assert(std::is_arithmetic<array_element_t>::value, "No arithmetic type");

        constexpr std::size_t N = decltype(std::declval<array_t>()){}.size();

        const array_t& sourceArray= member.getConstRef(m_initialObj);
        array_t& destArray= member.getRef(m_convertedObj);

        switch (m_ConvertMode)
        {
            case EConvertMode::NET_ORDER:
            {
                std::transform(sourceArray.begin(), sourceArray.end(), destArray.begin(),
                    [](const auto& a) { return host_to_network(a); });
                break;
            }
            case EConvertMode::HOST_ORDER:
            {
                std::transform(sourceArray.begin(), sourceArray.end(), destArray.begin(),
                    [](const auto& a) { return network_to_host(a); });
                break;
            }
        }

    }

    template<typename Member, std::enable_if_t<std::is_array_v<EtEndian::get_member_type<Member>>, int> = 0>
    void operator() (const Member& member)
    {
        // const char* name = member.getName();
        using array_t = EtEndian::get_member_type<Member>;
        using array_element_t = std::decay_t<decltype(std::declval<array_t>()[0])>;

        static_assert(std::is_arithmetic<array_element_t>::value, "No arithmetic type");

        constexpr std::size_t N = utils::array_count<utils::remove_cvref_t<array_t>>::value;

        const array_t& sourceArray= member.getConstRef(m_initialObj);
        array_t& destArray= member.getRef(m_convertedObj);

        switch (m_ConvertMode)
        {
            case EConvertMode::NET_ORDER:
            {
                for(int i = 0; i < N; i++) {
                    destArray[i] = host_to_network(sourceArray[i]);
                }
                break;
            }
            case EConvertMode::HOST_ORDER:
            {
                for(int i = 0; i < N; i++) {
                    destArray[i] = network_to_host(sourceArray[i]);
                }
                break;
            }
        }
    }


    template<typename Member, std::enable_if_t< std::is_same<EtEndian::get_member_type<Member>, std::string>::value, int> = 0>
    void operator()(const Member& member) noexcept
    {
        const std::string& sourceString = member.getConstRef(m_initialObj);
        std::string& destString = member.getRef(m_convertedObj);
        //No endian converson necessary because std::string is a byte array
        destString = std::string(sourceString);
        //destString.swap(sourceString);
    }

    const T& value() const noexcept
    {
        return m_initialObj;
    }

    const T& converted() const noexcept
    {
        return m_convertedObj;
    }

private:
    const EConvertMode  m_ConvertMode;
    const T             m_initialObj;
    T                   m_convertedObj;
};

}

#endif // _ENDIANCONVERTER_H_
