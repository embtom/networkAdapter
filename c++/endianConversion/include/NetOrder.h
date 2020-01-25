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

#ifndef _ENDIANORDER_H_
#define _ENDIANORDER_H_

#include <string>
#include <type_traits>
#include <array>
#include <EndianConverter.h>

namespace EtEndian
{

template <typename T>
using removeConstReference_t = std::remove_const_t<std::remove_reference_t<T>>;


template<typename T>
class CNetOrder
{
public:
    template<typename U, std::enable_if_t<!std::is_same<removeConstReference_t<U>, CNetOrder>::value, int> = 0>    
    CNetOrder(U &&obj) noexcept :
        m_converterFunc(EConvertMode::NET_ORDER, std::forward<removeConstReference_t<U>>(obj))
    {
        doForAllMembers<T>(m_converterFunc);
    }

    CNetOrder()                            = delete;      
    CNetOrder(CNetOrder const&)            = delete;
    CNetOrder& operator=(CNetOrder const&) = delete;

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
CNetOrder(U) -> CNetOrder<removeConstReference_t<U>>;
}


#endif //_NETORDER_H_