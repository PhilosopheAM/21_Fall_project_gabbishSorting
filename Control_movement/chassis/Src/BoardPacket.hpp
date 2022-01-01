#ifndef BOARDPACKET_HPP
#define BOARDPACKET_HPP

#include "Packet.hpp"
#include "CanMsgHandler.hpp"
#include "ChassisCtrlMsg.hpp"
#include "ChassisCtrlMsg2.hpp"
#include "GimbalRefMsg.hpp"

#define BOARD_CHANNEL_NUM 2

enum BoardPacketChannelIdType
{
    CIT_Channel0 = 0x120,
    CIT_Channel1 = 0x130,
};

class BoardPacket : public Packet
{
private:
    BoardPacketChannelIdType m_ChannelCanId;

public:
    BoardPacket(){}

    virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        Packet::Init(_id);
        m_ChannelCanId = _channelCanId;

        Registration();
    }

    virtual void Registration();
    virtual void SendPacket();
};

class ShooterStateSetPacket : public BoardPacket
{
public:
    ShooterStateSetPacket():BoardPacket(){}
    
	uint8_t shooter_state;
		
	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        shooter_state = 0;
    }
		
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class SCStateFdbPacket : public BoardPacket
{
public:
    SCStateFdbPacket():BoardPacket(){}
    
	float cap_voltage;
    float output_power;
    float output_voltage;

	bool receive_flag;
		
	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        cap_voltage = 0.0f;
		output_power = 0.0f;
		output_voltage = 0.0f;
    }
		
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class SCStateSetPacket : public BoardPacket
{
public:
    SCStateSetPacket():BoardPacket(){}
    
    uint8_t chassis_state;
    uint8_t relay_state;
	float max_power_set;

	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        chassis_state = 0;
		relay_state = 0;
		max_power_set = 0.0f;
    }
			
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class TestBoardPacket : public BoardPacket
{
public:
	TestBoardPacket(){}
    uint8_t m_testData;

    virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        m_testData = 0x00;
    }

    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class RobotStatePacket : public BoardPacket
{
public:
    RobotStatePacket(){}

    uint8_t chassis_level;
	uint8_t shooter_level;
	uint16_t shooter_heat;

	bool receive_flag;

	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        chassis_level = 0;
		shooter_level = 0;
		shooter_heat = 100;
    }
	
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class BoardPacketManager : public PacketManager, public CanMsgHandler
{
private:
    StreamChannel m_StreamChannel[BOARD_CHANNEL_NUM];
    CAN_TypeDef *m_pCan;

    TestBoardPacket m_testPacket;

	SCStateFdbPacket m_sc_state_fdb_packet;
	SCStateSetPacket m_sc_state_set_packet;

	ShooterStateSetPacket m_shooter_state_packet;

	RobotStatePacket m_robot_state_packet;

    ChassisCtrlMsg m_ctrlMsg;
    ChassisCtrlMsg2 m_ctrlMsg2;
    GimbalRefMsg m_gimbalRefMsg;

protected:
    virtual bool FlushSendBufferLow();

public:
    BoardPacketManager();

    TestBoardPacket& GetTestPacket() { return m_testPacket; }

    ChassisCtrlMsg& GetChassisCtrlMsg() { return m_ctrlMsg; }
    ChassisCtrlMsg2& GetChassisCtrlMsg2() { return m_ctrlMsg2; }
    GimbalRefMsg& GetGimbalRefMsg() { return m_gimbalRefMsg; }
	SCStateFdbPacket& GetSCFdb() { return m_sc_state_fdb_packet;}
	SCStateSetPacket& GetSCSet() { return m_sc_state_set_packet;} 
	ShooterStateSetPacket&  GetShooterState() {return m_shooter_state_packet;}
	RobotStatePacket& GetRobotStatePacket () {return m_robot_state_packet;}
	
    virtual void Update();
    virtual void Init();
    virtual void HandleNewCanRxMsg(CanRxMsg *_msg);

    static BoardPacketManager *Instance()
    {
        static BoardPacketManager instance;
        return &instance;
    }
};

#endif
