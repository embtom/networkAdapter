/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2020 Thomas Willetal
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

#ifndef _NETORDER_H_
#define _NETORDER_H_

//******************************************************************************
// Header

#include <string>
#include <type_traits>
#include <array>
#include <EndianConverter.h>
#include <templateHelpers.h>

namespace EtEndian
{

//*****************************************************************************
//! \brief CNetOrder
//!

template<typename T>
class CNetOrder
{
public:
    CNetOrder()                            = delete;
    CNetOrder(const CNetOrder&)            = default;
    CNetOrder& operator=(const CNetOrder&) = delete;

    template<typename U, std::enable_if_t<!std::is_same_v<utils::remove_cvref_t<U>, CNetOrder>, int> = 0>
    CNetOrder(U &&obj) noexcept :
        m_converterFunc(EConvertMode::NET_ORDER, std::forward<U>(obj))
    {
        doForAllMembers<T>(m_converterFunc);
    }

    const T& HostOrder() const noexcept
    {
        return m_converterFunc.value();
    }

    const T& NetworkOrder() const noexcept
    {
        return m_converterFunc.converted();
    }
private:
    ConverterFunc<T> m_converterFunc;
};

//https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
template<class U>
CNetOrder(U) -> CNetOrder<utils::remove_cvref_t<U>>;

//*****************************************************************************
//! \brief type trait to check if CHostOrder
//!

template<typename T>
struct is_net_order :  std::false_type{};

template<typename T>
struct is_net_order<CNetOrder<T>>
{
    static constexpr bool value = std::is_trivially_copyable_v<T>;
};

template <typename T>
inline constexpr bool is_net_order_v = is_net_order<T>::value;

}

#endif //_NETORDER_H_
