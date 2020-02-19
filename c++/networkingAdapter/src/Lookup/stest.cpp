	for (currIface = ifaces; currIface != 0; currIface = currIface->ifa_next)
	{
		try
		{
			if (!currIface->ifa_addr) continue;

			IPAddress address, subnetMask, broadcastAddress;
			unsigned family = currIface->ifa_addr->sa_family;
			switch (family)
			{
#if defined(POCO_OS_FAMILY_BSD)
			case AF_LINK:
			{
				struct sockaddr_dl* sdl = (struct sockaddr_dl*) currIface->ifa_addr;
				ifIndex = sdl->sdl_index;
				intf = NetworkInterface(ifIndex);
				setInterfaceParams(currIface, intf.impl());
				if ((result.find(ifIndex) == result.end()) && ((upOnly && intf.isUp()) || !upOnly))
					ifIt = result.insert(Map::value_type(ifIndex, intf)).first;
				break;
			}
#endif
			case AF_INET:
				ifIndex = if_nametoindex(currIface->ifa_name);
				ifIt = result.find(ifIndex);
				intf = NetworkInterface(ifIndex);
				setInterfaceParams(currIface, intf.impl());
				if ((ifIt == result.end()) && ((upOnly && intf.isUp()) || !upOnly))
					ifIt = result.insert(Map::value_type(ifIndex, intf)).first;

				address = IPAddress(*(currIface->ifa_addr));

				if (( currIface->ifa_flags & IFF_LOOPBACK ) == 0 && currIface->ifa_netmask)
					subnetMask = IPAddress(*(currIface->ifa_netmask));

				if (currIface->ifa_flags & IFF_BROADCAST && currIface->ifa_broadaddr)
					broadcastAddress = IPAddress(*(currIface->ifa_broadaddr));
				else if (currIface->ifa_flags & IFF_POINTOPOINT && currIface->ifa_dstaddr)
					broadcastAddress = IPAddress(*(currIface->ifa_dstaddr));
				else
					broadcastAddress = IPAddress();
				break;
#if defined(POCO_HAVE_IPv6)
			case AF_INET6:
				ifIndex = if_nametoindex(currIface->ifa_name);
				ifIt = result.find(ifIndex);
				intf = NetworkInterface(ifIndex);
				setInterfaceParams(currIface, intf.impl());
				if ((ifIt == result.end()) && ((upOnly && intf.isUp()) || !upOnly))
					ifIt = result.insert(Map::value_type(ifIndex, intf)).first;

				address = IPAddress(&reinterpret_cast<const struct sockaddr_in6*>(currIface->ifa_addr)->sin6_addr,
					sizeof(struct in6_addr), ifIndex);
				subnetMask = IPAddress(*(currIface->ifa_netmask));
				broadcastAddress = IPAddress();
				break;
#endif
			default:
				continue;
			}

			if (family == AF_INET
#ifdef POCO_HAVE_IPv6
			|| family == AF_INET6
#endif
			)
			{
				if ((upOnly && intf.isUp()) || !upOnly)
				{
					if ((ifIt = result.find(ifIndex)) != result.end())
						ifIt->second.addAddress(address, subnetMask, broadcastAddress);
				}
			}
		}
		catch (Poco::Exception&)
		{
		}
		catch (...)
		{
			if (ifaces) freeifaddrs(ifaces);
			throw;
		}
	}
	if (ifaces) freeifaddrs(ifaces);