
#include <BaseSocket.hpp>
#include <sys/socket.h>

using namespace EtNet;

class CSocketImpl : public EtNet::CBaseSocket
{
    public:
    CSocketImpl() = default;
    CSocketImpl(EtNet::ESocketMode opMode) :
        CBaseSocket(opMode)
    {}

    EtNet::ESocketMode testSocketMode()
    {
        int type, domain;
        int length = sizeof( int );
        int fd = getFd();
        getsockopt( fd, SOL_SOCKET, SO_TYPE, &type, (socklen_t*)&length);
        getsockopt( fd, SOL_SOCKET, SO_DOMAIN, &domain, (socklen_t*)&length);

        if ((type == SOCK_DGRAM) && (domain == PF_INET)) {
            return  ESocketMode::INET_DGRAM;
        }

        else if ((type == SOCK_STREAM) && (domain == PF_INET)) {
            return  ESocketMode::INET_STREAM;
        }

        else if ((type == SOCK_DGRAM) && (domain == PF_INET6)) {
            return  ESocketMode::INET6_DGRAM;
        }

        else if ((type == SOCK_STREAM) && (domain == PF_INET6)) {
            return  ESocketMode::INET6_STREAM;
        }

        else {
            return ESocketMode::NO_MODE;
        }
    }

    
};