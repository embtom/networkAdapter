#ifndef _COMPROTO_HPP_
#define _COMPROTO_HPP_

#include <stdint.h>
#include <string>
#include <NetOrder.h>
#include <HostOrder.h>

struct ComProto
{
    char info [10];
    uint32_t    data1;
    uint16_t    data2;
    uint32_t    disconnect;
};


template <>
inline auto EtEndian::registerMembers<ComProto>()
{
   return members(
      member("info",       &ComProto::info),
      member("data1",      &ComProto::data1),
      member("data2",      &ComProto::data2),
      member("disconnect", &ComProto::disconnect)
   );
}

#endif //_COMPROTO_HPP_