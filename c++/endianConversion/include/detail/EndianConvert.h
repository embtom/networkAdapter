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

#ifndef _NETCONVERT_H_
#define _NETCONVERT_H_

#include <endian.h>   // __BYTE_ORDER __LITTLE_ENDIAN
#include <type_traits>
#include <byteswap.h>

namespace EtEndian
{


template<typename T, std::enable_if_t<std::is_arithmetic<T>::value && 
                                     (sizeof(T) == 2), int> = 0>
constexpr T host_to_network (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap16(value); 
#else
    return value;
#endif

}

template <typename T, std::enable_if_t<std::is_arithmetic<T>::value && 
                                       sizeof(T) == 4, int> = 0>
constexpr T host_to_network (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap32(value);
#else
    return value;
#endif
}

template <typename T, std::enable_if_t<std::is_arithmetic<T>::value && 
                                       sizeof(T) == 8, int> = 0>
constexpr T host_to_network (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap64(value);  // __bswap_constant_64
#else
    return value;
#endif
}

template<typename T, std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
constexpr T host_to_network (T value) noexcept
{
    static_assert(std::is_arithmetic<T>::value, "No arithmetic type");
}

template <typename T>
constexpr T network_to_host (T value) noexcept
{
    return host_to_network<T>(value);
}

} //EtNet
#endif //_NETWORKORDER_H_
