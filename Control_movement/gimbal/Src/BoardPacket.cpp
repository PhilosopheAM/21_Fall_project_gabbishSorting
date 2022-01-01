#include "BoardPacket.hpp"

#include "CanMsgDispatcher.hpp"

void BoardPacket::Registration()
{
    uint32_t _channelId = 0;
    switch (m_ChannelCanId)
    {
    case CIT_Channel0:
        _channelId = 0;
        break;
    case CIT_Channel1:
        _channelId = 1;
        break;

    default:
        break;
    }
    BoardPacketManager::Instance()->PacketHandlerRegistration(_channelId, m_ProtocolId, this);
}

void BoardPacket::SendPacket()
{
    BoardPacketManager::Instance()->SendPacket(m_Buffer, m_DataLen + 8);
}

void TestBoardPacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
    NotifyAll((void*)&m_testData);
}

void TestBoardPacket::SendPacket()
{
    SerializePacket(m_OutputStream);
    BoardPacket::SendPacket();
}

template<typename Stream>
void TestBoardPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 1;

    SerializeHeader(stream);

    stream.SerializeU8(m_testData);

    SerializeCrc16(stream);
}

void FireSetPacket::SendPacket()
{  
	SerializePacket(m_OutputStream);
    BoardPacket::SendPacket();
}

void FireSetPacket::OnPacketReceived()
{
	SerializePacket(m_InputStream);
	receive_flag =  true;
}

template<typename Stream>
void FireSetPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 3;
    SerializeHeader(stream);
    stream.SerializeU8(fire_state_set);
	stream.SerializeU16(speed_set);
    SerializeCrc16(stream);
}

void AutoReloadPacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
}

void AutoReloadPacket::SendPacket()
{
    SerializePacket(m_OutputStream);
    BoardPacket::SendPacket();
}

template<typename Stream>
void AutoReloadPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 1;

    SerializeHeader(stream);

    stream.SerializeU8(reload_set);

    SerializeCrc16(stream);
}

BoardPacketManager::BoardPacketManager() : PacketManager(BOARD_CHANNEL_NUM), m_pCan(CAN2)
{
    m_pChannel = m_StreamChannel;
}

void BoardPacketManager::Init()
{
    CanMsgDispatcher::Instance()->RegisterHandler(m_pCan, CIT_Channel0, this);
    CanMsgDispatcher::Instance()->RegisterHandler(m_pCan, CIT_Channel1, this);
    PacketManager::Init();

    m_testPacket.Init(CIT_Channel0, 0x99);
	m_fire_set_packet.Init(CIT_Channel0,0x50);
    
    m_ctrlMsg.Init(CAN2, 0x250);
    m_ctrlMsg2.Init(CAN2, 0x251);
    m_gimbalRefMsg.Init(CAN2, 0x252);
	m_auto_reload_packet.Init(CIT_Channel0,0x80);
}

bool BoardPacketManager::FlushSendBufferLow()
{
	//bug
    CanManager::Instance()->CanTransmit(m_pCan, CIT_Channel0, m_SendBuffer, m_SendBufferLength);
    return true;
}

void BoardPacketManager::HandleNewCanRxMsg(CanRxMsg *_msg)
{
    if (_msg->StdId == CIT_Channel0)
    {
        Enqueue(0, _msg->Data, _msg->DLC);
        return;
    }
    Enqueue(1, _msg->Data, _msg->DLC);
}
