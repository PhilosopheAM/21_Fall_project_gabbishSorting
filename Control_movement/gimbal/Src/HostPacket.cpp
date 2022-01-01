#include "HostPacket.hpp"

void HostPacket::Registration()
{
    HostPacketManager::Instance()->PacketHandlerRegistration(0, m_ProtocolId, this);
}

void HostPacket::SendPacket()
{
    HostPacketManager::Instance()->SendPacket(m_Buffer, m_DataLen + 8);
}

HostPacketManager::HostPacketManager() : PacketManager(1)
{
    m_pChannel = &m_StreamChannel;
}

void HostPacketManager::Init()
{
    bsp_host_com_init();
    PacketManager::Init();

    
}

bool HostPacketManager::FlushSendBufferLow()
{
    if (bsp_host_com_send_busy())
    {
        return false;
    }

    bsp_host_com_send(m_SendBuffer, m_SendBufferLength);
    return true;
}
