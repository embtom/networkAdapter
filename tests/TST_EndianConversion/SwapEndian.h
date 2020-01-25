
#include <type_traits>
#include <array>

namespace EtTest
{

template <typename T, std::enable_if_t<std::is_arithmetic<T>::value,int> = 0>
T swapEndian(T value)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    union {
        T val;
        std::array<uint8_t ,sizeof(T)> raw;
    } src, dst;

    src.val = value;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    return dst.val;
#else
    return value;
#endif
}

}