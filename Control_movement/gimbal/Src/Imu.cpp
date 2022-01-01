#include "Imu.hpp"
#include "Stream.hpp"
#include "Math.hpp"

#include "Time.hpp"

uint8_t Imu::ImuCount = 0;

Imu::Imu():SensorEntity(ECT_Imu),
           m_BufferIndex(0),
           m_Front(0),
           m_Rear(0),
           m_ImuId(ImuCount)
{
    ++ImuCount;
}

void Imu::Init()
{
    bsp_imu_init();
    SetDefaultTicksToUpdate(2);
}

void Imu::Update()
{
    while (m_Rear != m_Front)
    {
        if(m_BufferQueue[m_Front] == 0x55 || m_BufferIndex > 10)
        {
            ResolvePacket();
        }
        m_FrameBuffer[m_BufferIndex] = m_BufferQueue[m_Front];
        ++m_BufferIndex;
        ++m_Front;
    }

    if(Time::GetTick() - LastMileStone > 1000)
    {
        LastMileStone = Time::GetTick();
        real_freq = freq;
        freq = 0.0f;
    }
}

void Imu::ResolvePacket()
{
    uint8_t _checkSum = 0x00;
    for(int i = 0;i < 10;++i)
    {
        _checkSum += m_FrameBuffer[i];
    }
    if(_checkSum == m_FrameBuffer[10])
    {
        HandlePacket();
    }
}

void Imu::HandlePacket()
{
    m_BufferIndex = 0;
    uint8_t _packetId;
    uint8_t* ptr;
    int16_t temp;
    _packetId = m_FrameBuffer[1];

    switch (_packetId)
    {
    case 0x52:
        ptr = &m_FrameBuffer[2];
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_WX = (float)(temp) / 32768.0f * 34.90658504f;
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_WY = (float)(temp) / 32768.0f * 34.90658504f;
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_WZ = (float)(temp) / 32768.0f * 34.90658504f;
        break;

    case 0x53:
        freq++;
        ptr = &m_FrameBuffer[2];
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_Roll = (float)(temp) / 32768.0f * Math::Pi;
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_Pitch = (float)(temp) / 32768.0f * Math::Pi;
        temp = ArrayStreamHelper::ReadInt16(ptr);
        m_YawRaw = (float)(temp) / 32768.0f * Math::Pi;
        m_Yaw = Math::LoopFloatConstrain(m_YawRaw - m_ZeroYaw, -Math::Pi, Math::Pi);
        break;
    
    default:
        break;
    }
}

void Imu::Enqueue(uint8_t _data)
{
    m_BufferQueue[m_Rear] = _data;

    // Buffer len is 256, therefore auto overflow
    ++m_Rear;
}

void Imu::Enqueue(uint8_t* _pData, uint32_t _len)
{
    for (uint32_t i = 0; i < _len; ++i)
    {
        Enqueue(_pData[i]);
    }
}
