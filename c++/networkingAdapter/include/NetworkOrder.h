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

#ifndef _NETWORKORDER_H_
#define _NETWORKORDER_H_

#include <endian.h>   // __BYTE_ORDER __LITTLE_ENDIAN
#include <type_traits>
#include <byteswap.h>

namespace EtNet
{

template<typename T, std::enable_if_t<sizeof(T) == 2, int> = 0>
constexpr T htonT (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap16(value); 
#else
    return value;
#endif

}

template <typename T, std::enable_if_t<sizeof(T) == 4, int> = 0>
constexpr T htonT (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap32(value);
#else
    return value;
#endif
}

template <typename T, std::enable_if_t<sizeof(T) == 8, int> = 0>
constexpr T htonT (T value) noexcept
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return __builtin_bswap64(value);  // __bswap_constant_64
#else
    return value;
#endif
}

template <typename T>
constexpr T ntohT (T value) noexcept
{
    return htonT<T>(value);
}

} //EtNet
#endif //_NETWORKORDER_H_
