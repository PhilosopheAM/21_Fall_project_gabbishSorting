#ifndef IMU_HPP
#define IMU_HPP

#include "bsp_imu.h"
#include "RobotEngine.hpp"

class Imu : public SensorEntity
{
private:
    float m_YawRaw;
    float m_Yaw;
    float m_Pitch;
    float m_Roll;
    float m_WX;
    float m_WY;
    float m_WZ;
    float m_ZeroYaw;
    uint8_t m_FrameBuffer[64];
    uint8_t m_BufferQueue[256];
    uint8_t m_BufferIndex;
    uint8_t m_Front;
    uint8_t m_Rear;
    uint8_t m_ImuId;
    static uint8_t ImuCount;

    uint32_t LastMileStone;
    float freq;
    float real_freq;

    void ResolvePacket();
    void HandlePacket();
public:
    Imu();

    uint8_t GetImuId(){ return m_ImuId; }

    virtual void Init();
    virtual void Update();

    void Enqueue(uint8_t _data);
    void Enqueue(uint8_t* _pData, uint32_t _len);

    float YawRaw(){ return m_YawRaw; }
    float Yaw(){ return m_Yaw; }
    float Pitch(){ return m_Pitch; }
    float Roll(){ return m_Roll; }
    float WX(){ return m_WX; }
    float WY(){ return m_WY; }
    float WZ(){ return m_WZ; }

    void SetYawZero(){ m_ZeroYaw = m_YawRaw; }
};

#endif
