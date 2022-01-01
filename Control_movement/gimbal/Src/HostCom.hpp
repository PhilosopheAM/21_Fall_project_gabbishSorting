#ifndef HOSTCOM_HPP
#define HOSTCOM_HPP

#include "HostPacket.hpp"

class HostGimbalTargetAnglePacket : public HostPacket
{
public:
    HostGimbalTargetAnglePacket(){}
    
    float m_yaw;
    float m_pitch;
    uint8_t m_found;
    uint8_t m_fire;
    uint16_t m_seq;
					
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class HostEchoPacket : public HostPacket
{
public:
    HostEchoPacket(){}
    
    uint16_t echoSeq;
					
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class HostGimbalFdbAnglePacket : public HostPacket
{
public:
    HostGimbalFdbAnglePacket():
        m_receive_flag(false){}
    
    float m_yaw;
    float m_pitch;

    bool m_receive_flag;
					
    virtual void OnPacketReceived();
    virtual void SendPacket();
	template<typename Stream> void SerializePacket(Stream &stream);
};

class HostCom 
{
private:
    HostCom();
    uint32_t time_count;
public:
    HostGimbalTargetAnglePacket hostGimbalTargetAnglePacket;
	HostGimbalFdbAnglePacket hostGimbalFdbAnglePacket;
    HostEchoPacket hostEchoPacket;

    uint32_t lastSecRecPacketNum;
    uint32_t lastMileStonePacketNum;

    void Init();
    void Update();

    ~HostCom(){;}

    static HostCom* Instance()
    {
        static HostCom instance;
        return &instance;
    }
};

#endif
