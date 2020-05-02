# networkadapter

This library provides simplified C++ posix sockets abstraction layer for
TCP and UDP communication. It also contains a handy c++ endian converter, based
on class member reflection.

The dependencies are:

| Lib           | Link |
| ------------- | ------------- |
| utilsCpp      | https://github.com/embtom/utilsCpp |
| docopt        | http://docopt.org |
| googletest    | https://github.com/google/googletest |

## Cmake library import
The networkadapter library provides exported cmake targets and can be imported by another component. After the installation of the networkadapter library it can be added with follwing
cmake commands:

```cmake
# Check if the development package of the library is available and import it
find_package(networkadapter REQUIRED)

# link library to the networkadapter
target_link_libraries(usage1 PRIVATE EMBTOM::networkadapter)
```


## Examples

Some Examples are available how to use this
TCP Server client example is located at "networkadapter/examples" and more examples are available.

#### 1. EXA_HostName
Lookup of all available Ip addresses by hostname
```console
EXA_HostName --host tom-LIFEBOOK
IPv4 Addresses:
192.168.1.80
IPv6 Addresses:
xxx:xx:971b:a700:4cf2:a936:4a93:xxx
```

#### 2. EXA_InterfaceLookup
Lookup of all Ip addresses ordered by network devices
```console
EXA_HostName --host tom-LIFEBOOK
IPv4 Addresses:
192.168.1.80
IPv6 Addresses:
xxx:xx:971b:a700:4cf2:a936:4a93:xxx
```

#### 3. EXA_Tcp
Example implementation of a TCP Client and Server connection

```console
EXA_TcpServer
Wait for connection
Connected by: 127.0.0.1
 info                Hallo
 data1               ffaa0000
 data2               aa00
 disconnect          0
 EXA_TcpServer
----
 info                Hallo
 data1               ffaa0009
 data2               aa09
 disconnect          1
----
Disconntected
```

```console
EXA_TcpClient -c localhost
try to connect to: localhost
 info                HALLOEcho
 data1               ffaa0000
 data2               aa00
 disconnect          0
----
 info                HALLOEcho
 data1               ffaa0009
 data2               aa09
 disconnect          1
----
finish
```

#### 3. EXA_Udp
Example implementation of a UDP Client and Server connection

```console
EXA_UdpServer
Connected from: 127.0.0.1
 info                Hallo
 data1               ffaa0000
 data2               aa00
 disconnect          0
```

```console
EXA_UdpClient -c localhost
try to connect to: localhost
DefaultRecive
 info                HALLOEcho
 data1               ffaa0000
 data2               aa00
 disconnect          0
```

#### 4. EXA_UdpBroadcast
Example implementation of a UDP broadcast reciver and transmitter.
In this example the transmitter sends a broadcast message with a specified broadcast address
into the network and the reciver response to the origin transmitter Ip.

```console
EXA_BroadServer
Connected from: 192.168.1.22
 info                Hallo
 data1               ffaa0000
 data2               aa00
 disconnect          0
```

```console
EXA_Broadclient
Broadcast Address to send is: 192.168.1.255
DefaultRecive
 info                HALLOEcho
 data1               ffaa0000
 data2               aa00
 disconnect          0
```

#### 3. Detailed consideration based on example EXA_Tcp

The EXA_Tcp uses the networkadapter integrated endian converter. It is responsible to convert the protocol payload form host-byte-order to network-byte-order or vice versa. (networkadapter/example/Proto).

```cmake
# Check if the development package is available and then import it
find_package(endianconversion REQUIRED)

add_library(ComProto INTERFACE)

target_include_directories (ComProto
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# link library to the networkadapter integraded endianconverter
target_link_libraries(ComProto INTERFACE
    EMBTOM::endianconversion
)
```

```cpp
#include <NetOrder.h>
#include <HostOrder.h>


// Declaration of structure used as protocol info
struct ComProto
{
    char info [10];
    uint32_t    data1;
    uint16_t    data2;
    uint32_t    disconnect;
};

// template for class members registrations and is specialized for ComProto protocol structure.
// Used for endian conversion by reflection
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
```



The Tcp Server can look:
```cpp
#include <iostream>
#include <iomanip>
#include <array>
#include <span.h>
#include <BaseSocket.hpp>
#include <Tcp/TcpServer.hpp>
#include <Tcp/TcpDataLink.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 5001;

using namespace EtNet;

int main(int argc, char *argv[])
{
    // Creaton of a IPv4 Udp Socket and apply the socketoption SO_REUSEADDR
    auto baseSocket = CBaseSocket::SoReuseSocket(
                      CBaseSocket(ESocketMode::INET_DGRAM));
    CTcpServer CTcpServer(std::move(baseSocket), PORT_NUM);

    CTcpDataLink DataLink;
    CIpAddress peerIp;
    while (true)
    {
        // Wait for connection of a TcpClient and returns the
        // peerIp  : Ip address ot the counterpart.
        // DataLink: Object to transmit and recive with the Client established the connection
        std::cout<< "Wait for connection" << std::endl;
        std::tie(DataLink,peerIp) = CTcpServer.waitForConnection();
        std::cout << "Connected by: " << peerIp.toString() << std::endl;

        bool serverActive = true;
        while(serverActive)
        {
            //Setup recive Buffer with endianconverter. (ComProto have to be registerd by "registerMembers")
            EtEndian::CHostOrder<ComProto> rx;
            DataLink.recive(rx);

            // Convert recived data form Network-byte-order to Host-byte-order
            const ComProto& rxData = rx.HostOrder();

            // The same mechanism used at endian conversion could be used to plot the content of data
            EtEndian::doForAllMembers<ComProto>(
                [&rxData](const auto& member)
            {
                using MemberT = EtEndian::get_member_type<decltype(member)>;
                std::cout << std::hex << " " << std::left
                          << std::setw(20) << member.getName()
                          << std::setw(20) << member.getConstRef(rxData)
                          << std::endl;
            });
            std::cout << "----" << std::endl;

            //Modify recived data and transmit it again
            ComProto txData;
            int i;
            for(i = 0; i < strlen(rxData.info); i++) {
                txData.info[i] = toupper(rxData.info[i]);
            }
            memcpy(&txData.info[i],"Echo",4);
            txData.data1 = rxData.data1;
            txData.data2 = rxData.data2;
            txData.disconnect = rxData.disconnect;
            if(rxData.disconnect) {
                std::cout << "Disconntected" << std::endl;
                serverActive = false;
            }

            // The convert the date to network-byte order again and transmit it.
            DataLink.send(EtEndian::CNetOrder(txData));
        }
    }
}
```
Tcp Client
```cpp
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <string>
#include <map>
#include <NetOrder.h>
#include <HostOrder.h>
#include <docopt.h>
#include <span.h>
#include <threadLoop.h>
#include <BaseSocket.hpp>
#include <Tcp/TcpClient.hpp>
#include <Tcp/TcpDataLink.hpp>
#include <ComProto.hpp>

constexpr int PORT_NUM = 5001;

using namespace EtNet;
using namespace EtEndian;

//*****************************************************************************
//! \brief EXA_TcpClient
//!

int main(int argc, char *argv[])
{
    // docopt is used to describe the command line interface
    constexpr std::string_view docOptCmd =
        R"(EXA_TcpClient.
            Usage:
            EXA_TcpClient (-c | --connect) <hostname>
            EXA_TcpClient (-h | --help)
            EXA_TcpClient --version
            Options:
            -h --help     Show this screen.
            --version     Show version.
        )";

    constexpr auto networkAdapterVersion = "networkAdapter " NETWORKING_ADAPTER_VERSION;
    using ArgMap_t = std::map<std::string, docopt::value>;
    ArgMap_t args = docopt::docopt(std::string(docOptCmd),
                                   { argv + 1, argv + argc },
                                   true,
                                   networkAdapterVersion);
    if (!(args["<hostname>"])) {
        return 0;
    }

    // Creaton of a IPv4 Udp Socket and apply the socketoption SO_REUSEADDR
    auto baseSocket = CBaseSocket::SoReuseSocket(
                      CBaseSocket(ESocketMode::INET_DGRAM));
    CTcpClient TcpClient(std::move(baseSocket));

    std::string hostName(args["<hostname>"].asString());
    std::cout << "try to connect to: " << hostName << std::endl;

    // try to connect to conect to a server with hostname and port
    EtNet::CTcpDataLink a;
    try {
        a = TcpClient.connect(hostName, PORT_NUM);
    }
    catch(const std::exception& e) {
        std::cout << "Failed to establish connection: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    //setup of the threaded recive loop
    unsigned rcvCount {0};
    auto rcvFunc = [&a, &rcvCount]()
    {
        EtEndian::CHostOrder<ComProto> rx;

        // the recive routine is blocking if no new data is available and with
        // the assistance of "unblockRecive" the recive routine can unblock the
        // thread loop can be prepared for joining.
        if(CTcpDataLink::ERet::UNBLOCK == a.recive(rx))
        {
            std::cout << "finish" << std::endl;
            return true;
        }

        // Convert recived data form Network-byte-order to Host-byte-order
        const ComProto& rxData = rx.HostOrder();

        // The same mechanism used at endian conversion could be used to plot the content of data
        EtEndian::doForAllMembers<ComProto>(
            [&rxData](const auto& member)
        {
            using MemberT = EtEndian::get_member_type<decltype(member)>;
            std::cout << std::hex << " " << std::left
                      << std::setw(20) << member.getName()
                      << std::setw(20) << member.getConstRef(rxData)
                      << std::endl;
        });
        std::cout << "----" << std::endl;
        return false;
    };

    // Create of a threaded receive loop (helper util provided by utilsCpp)
    utils::CThreadLoop rcvLoop (rcvFunc, "RX_Worker");
    rcvLoop.start(std::chrono::milliseconds::zero());

    //Prepare data to transmit
    ComProto tx {"Hallo", 0 ,0, 0};
    for (int i = 0; i < 10; i++)
    {
        tx.data1 = 0xffaa0000 + i;
        tx.data2 = 0xAA00 + i;
        a.send(CNetOrder(tx));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    tx.disconnect = 1;

     // The convert the date to network-byte order and transmit it.
    a.send(CNetOrder(tx));

    std::this_thread::sleep_for(std::chrono::seconds(5));

    // unblock receive by activation of the cancel socket
    a.unblockRecive();

    // blocking wait until the thread receive working is joined
    rcvLoop.waitUntilFinished();

    return EXIT_SUCCESS;
}
```

## Installation

Two approaches are available to install the networkadapter library. At the first on the dependencies could be installed
dedicated and the second one the packages could be installed by the package manger

## Installation without package manager

The install prefix is "/usr/local".

#### 1. googletest
```console
git clone https://github.com/google/googletest.git
cd googletest
git checkout release-1.10.0
mkdir -p build && cd build
cmake ../ -Dgmock_build_tests=ON -Dgtest_build_tests=ON -DPOSITION_INDEPENDENT_CODE=true -DCMAKE_INSTALL_PREFIX=/usr/local
make -j4
sudo make intall
```

#### 2. utilsCpp
```console
git clone https://github.com/embtom/utilsCpp.git
cd utilsCpp
git checkout v1.2.4
mkdir -p build && cd build
cmake ../ -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -DUTILSCPP_BUILD_TEST=ON -DCMAKE_INSTALL_PREFIX=/usr/local
make -j4
sudo make install
```

#### 2. networkadapter
```console
git clone https://github.com/embtom/networkadapter.git
cd networkadapter
git checkout v0.9.6
mkdir -p build && cd build
cmake ../ -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -DNETWORKINGADAPTER_BUILD_TEST=ON -DNETWORKINGADAPTER_BUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=/usr/local
make -j4
sudo make install
```


## Installation with the assistance of debian packages

At the networkadapter provides a build environment, based on a docker container, to package the networkadapter library and their dependencies as debian packages.
https://docs.docker.com/engine/install/ubuntu/

The container is based on the debian package builder (pbuilder) and
reprepo as debian package mirror.

Some helper scripts to setup the build container, package the dependencies and build the networkadapter are located within the "scripts" directory.

| Script                         | Task                               |
| ------------------------------ | ---------------------------------- |
| 010_docker_builder_create.sh   | Setup the dockercontainer to build |
| 020_docker_build_depend.sh     | Build the required dependencies as debian packages |
| 030_docker_build_netadapter.sh | Build the networkadapter as debian package |

#### Artifacts:

* googletest artifacts: "networkadapter/container/share_artifacts/googletest"

```console
google-mock_1.10.0.20191003-1_amd64.deb
googletest_1.10.0.20191003-1_all.deb
googletest_1.10.0.20191003-1_amd64.buildinfo
googletest_1.10.0.20191003-1_amd64.changes
googletest_1.10.0.20191003-1.dsc
googletest_1.10.0.20191003-1.tar.xz
googletest-tools_1.10.0.20191003-1_amd64.deb
libgmock-dev_1.10.0.20191003-1_amd64.deb
libgtest-dev_1.10.0.20191003-1_amd64.deb
```
* utilscpp artifacts: "networkadapter/container/share_artifacts/utilscpp"

```console
utilscpp_1.2.4_amd64.buildinfo
utilscpp_1.2.4_amd64.changes
utilscpp_1.2.4.dsc
utilscpp_1.2.4.tar.xz
utilscpp-dbg_1.2.4_amd64.deb
utilscpp-dev_1.2.4_amd64.deb
```
* networkadapter artifacts "networkadapter/build_deb"

```console
networkadapter_0.9.6_amd64.buildinfo
networkadapter_0.9.6_amd64.changes
networkadapter_0.9.6_amd64.deb
networkadapter_0.9.6.dsc
networkadapter_0.9.6.tar.xz
networkadapter-dbg_0.9.6_amd64.deb
networkadapter-dev_0.9.6_amd64.deb
networkadapter-examples_0.9.6_amd64.deb
```
