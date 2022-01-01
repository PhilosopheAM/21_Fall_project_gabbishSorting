#ifndef HOSTPACKET_HPP
#define HOSTPACKET_HPP

#include "bsp_host_com.h"
#include "Packet.hpp"

class HostPacket : public Packet
{
public:
    HostPacket(){}

    virtual void Init(uint8_t _id)
    {
        Packet::Init(_id);

        Registration();
    }

    virtual void Registration();
    virtual void SendPacket();
};

class HostPacketManager : public PacketManager
{
private:
    StreamChannel m_StreamChannel;

protected:
    virtual bool FlushSendBufferLow();

public:
    HostPacketManager();

    virtual void Init();

    static HostPacketManager* Instance()
    {
        static HostPacketManager instance;
        return &instance;
    }
};

#endif
