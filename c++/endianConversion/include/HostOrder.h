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

#ifndef _HOSTORDER_H_
#define _HOSTORDER_H_

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
//! \brief CHostOrder
//!
template<typename T>
class CHostOrder
{
public:
    using class_type = T;

    CHostOrder(const CHostOrder&)             = default;
    CHostOrder& operator=(const CHostOrder &) = delete;

    template<typename U, std::enable_if_t<!std::is_same<utils::remove_cvref_t<U>, CHostOrder>::value, int> = 0>
    CHostOrder(U &&obj) noexcept :
        m_doItOnce(false),
        m_converterFunc(EConvertMode::HOST_ORDER, std::forward<U>(obj))
    {  }

    CHostOrder() noexcept :
        m_doItOnce(false),
        m_converterFunc(EConvertMode::HOST_ORDER)
    {  }

    const T& HostOrder() noexcept
    {
        if (!m_doItOnce) {
            doForAllMembers<class_type>(m_converterFunc);
            m_doItOnce = true;
        }
        return m_converterFunc.converted();
    }

    const T& NetworkOrder() const noexcept
    {
        return m_converterFunc.value();
    }

    T& object() noexcept
    {
        return m_converterFunc.object();
    } 
private:
    bool m_doItOnce;
    ConverterFunc<T> m_converterFunc;
};

//https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
template<class U>
CHostOrder(U) -> CHostOrder<utils::remove_cvref_t<U>>;


//*****************************************************************************
//! \brief type trait to check if CHostOrder
//!

template<typename T>
struct is_host_order :  std::false_type{};

template<typename T>
struct is_host_order<EtEndian::CHostOrder<T>> : std::true_type
{
//    static constexpr bool value = true; //std::is_trivially_copyable_v<T>;
};

template <typename T>
inline constexpr bool is_host_order_v = is_host_order<T>::value;

}

#endif //_HOSTORDER_H_
