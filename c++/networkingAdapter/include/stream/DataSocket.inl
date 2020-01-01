

#undef min
#undef max
#include <stdexcept>
#include <error_msg.hpp>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

namespace EtNet
{

template<typename F>
std::size_t CDataSocket::recive(uint8_t* buffer, std::size_t len, F scanForEnd)
{
    if (getFd() == 0)
    {
        throw std::logic_error(utils::buildErrorMessage("DataSocket::", __func__, ": accept called on a bad socket object (this object was moved)"));
    }

    std::size_t dataRead  = 0;
    while(dataRead < len)
    {
        // The inner loop handles interactions with the socket.
        std::size_t get = ::read(getFd(), buffer + dataRead, len - dataRead);
        if (get == static_cast<std::size_t>(-1))
        {
            switch(errno)
            {
                case EBADF:     [[fallthrough]];
                case EFAULT:    [[fallthrough]];
                case EINVAL:    [[fallthrough]];
                case ENXIO:
                {
                    // Fatal error. Programming bug
                    throw std::domain_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: critical error: ", strerror(errno)));
                }

                case EIO:       [[fallthrough]];
                case ENOBUFS:   [[fallthrough]];
                case ENOMEM:
                {
                   // Resource acquisition failure or device error
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: resource failure: ", strerror(errno)));
                }
                case EINTR:     [[fallthrough]];
                    // TODO: Check for user interrupt flags.
                    //       Beyond the scope of this project
                    //       so continue normal operations.
                case ETIMEDOUT: [[fallthrough]];
                case EAGAIN:
                {
                    // Temporary error, retry
                    continue;
                }
                case ECONNRESET:[[fallthrough]];
                case ENOTCONN:
                {
                    // Connection broken.
                    // Return the data we have available and exit
                    // as if the connection was closed correctly.
                    get = 0;
                    break;
                }
                default:
                {
                    throw std::runtime_error(utils::buildErrorMessage("DataSocket::", __func__, ": read: returned -1: ", strerror(errno)));
                }
            }
        }
        if (get == 0)
        {
            break;
        }
        dataRead += get;
        if (scanForEnd(dataRead))
        {
            break;
        }
    }

    return dataRead;
}

}