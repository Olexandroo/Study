// IPCalc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<sstream>
#include<string>

typedef uint32_t addr_t;
const size_t g_bytesInAddr = 4;

addr_t StrAddrToByte(const std::string & addr)
{
    std::stringstream ss(addr);
    addr_t ret = 0;
    for (size_t i = 0; i < g_bytesInAddr; ++i)
    {
        addr_t byte = 0;
        ss >> byte;
        ret |= byte << ((g_bytesInAddr - i -1) * 8);
        ss.get();
    }
    return ret;
}

std::string ByteAddrToStr(const addr_t & addr)
{
    std::stringstream ss;
    for (size_t i = 0; i < g_bytesInAddr; ++i)
    {
        uint8_t byte = addr >> ((g_bytesInAddr - i -1) * 8);
        ss << (int)byte;
        if (i != g_bytesInAddr - 1)
        {
            ss << '.';
        }
    }
    return ss.str();
}

addr_t GetNetwork(const addr_t & addr, const addr_t & mask)
{
    return mask & addr;
}

addr_t GetBroadcast(const addr_t & addr, const addr_t & mask)
{
    return GetNetwork(addr, mask) | ~mask;
}

size_t CountSetBits(uint32_t addr)
{
    size_t count = 0;
    while (addr)
    {
        count += addr & 1;
        addr >>= 1;
    }
    return count;
}

inline addr_t GetHostMin(const addr_t & addr, const addr_t & mask)
{
    return GetNetwork(addr, mask) +1;
}

inline addr_t GetHostMax(const addr_t & addr, const addr_t & mask)
{
    return GetNetwork(addr, mask) + ~mask -1;
}

inline addr_t GetWildcard(const addr_t & mask)
{
    return ~mask;
}

addr_t GetMask(const std::string & strSize)
{
    std::stringstream ss(strSize);
    size_t size = 0;
    ss >> size;
    addr_t ret = 0;
    for (size_t i = 0; i < size; ++i)
    {
        ret |= 1 << (sizeof(addr_t) * 8 - i - 1);
    }
    return ret;
}

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cerr << "Missing ip address"
            << "Usage: IPCalc.exe 10.30.40.123 255.255.255.0\n"
            << "       IPCalc.exe 10.30.40.123 /12\n";
        return 1;
    }
    addr_t mask = 0;
    if (argv[2][0] == '/')
        mask = GetMask(&argv[2][1]);
    else
        mask = StrAddrToByte(argv[2]);
    addr_t ip = StrAddrToByte(argv[1]);
    addr_t subnet = GetNetwork(ip, mask);
    addr_t broadcast = GetBroadcast(ip, mask);
    size_t setBits = CountSetBits(mask);
    addr_t hostMin = GetHostMin(ip, mask);
    addr_t hostMax = GetHostMax(ip, mask);
    addr_t wildcart = GetWildcard(mask);

    std::cout << "Address:\t"<< ByteAddrToStr(ip) << std::endl;
    std::cout << "Netmask:\t"<<ByteAddrToStr(mask) << " = " << setBits << std::endl;
    std::cout << "Wildcard:\t" << ByteAddrToStr(wildcart) << std::endl;
    std::cout << "Network:\t" << ByteAddrToStr(subnet) << " /"<< setBits << std::endl;
    std::cout << "Broadcast:\t" << ByteAddrToStr(broadcast) << std::endl;
    std::cout << "HostMin:\t" << ByteAddrToStr(hostMin) << std::endl;
    std::cout << "HostMax:\t" << ByteAddrToStr(hostMax) << std::endl;
    std::cout << "Hosts/Net:\t" << wildcart - 1 << std::endl;
    return 0;
}

