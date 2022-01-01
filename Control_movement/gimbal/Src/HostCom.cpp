#include "HostCom.hpp"
#include "Buzzer.hpp"

HostCom::HostCom()
{

}

void HostCom::Init()
{
	hostGimbalTargetAnglePacket.Init(0x05);
    hostGimbalFdbAnglePacket.Init(0x14);
    hostEchoPacket.Init(0x06);
}

void HostCom::Update()
{
    switch (time_count % 5)
    {
    case 0:
		// hostGimbalFdbAnglePacket.SendPacket();
        break;
    case 1:
        break;

    default:
        break;
    }

    time_count++;
    if(Time::GetTick() % 100 == 0)
    {
        lastSecRecPacketNum = lastMileStonePacketNum;
        lastMileStonePacketNum = 0;
    }
}

void HostGimbalTargetAnglePacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
    ++HostCom::Instance()->lastMileStonePacketNum;

    HostCom::Instance()->hostEchoPacket.echoSeq = m_seq;
    HostCom::Instance()->hostEchoPacket.SendPacket();

    Buzzer::Instance()->On();
    Buzzer::Instance()->SetFreq(m_seq);
}

void HostGimbalTargetAnglePacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

template<typename Stream>
void HostGimbalTargetAnglePacket::SerializePacket(Stream &stream)
{
    m_DataLen = 12;

    SerializeHeader(stream);

    stream.SerializeFloat(m_yaw);
    stream.SerializeFloat(m_pitch);
    stream.SerializeU8(m_found);
    stream.SerializeU8(m_fire);
    stream.SerializeU16(m_seq);

    SerializeCrc16(stream);
}

void HostEchoPacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
}

void HostEchoPacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

template<typename Stream>
void HostEchoPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 2;

    SerializeHeader(stream);

    stream.SerializeU16(echoSeq);

    SerializeCrc16(stream);
}

void HostGimbalFdbAnglePacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
	m_receive_flag = true;
}

void HostGimbalFdbAnglePacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

template<typename Stream>
void HostGimbalFdbAnglePacket::SerializePacket(Stream &stream)
{
    m_DataLen = 8;

    SerializeHeader(stream);

    stream.SerializeFloat(m_yaw);
    stream.SerializeFloat(m_pitch);

    SerializeCrc16(stream);
}
