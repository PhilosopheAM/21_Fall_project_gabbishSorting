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

class FireSetPacket : public BoardPacket
{
public:
    FireSetPacket(){}
    
    uint8_t fire_state_set;
	uint16_t speed_set;

	bool receive_flag;
	
	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        fire_state_set = 0;
		speed_set = 0;
    }
	
    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class AutoReloadPacket : public BoardPacket
{
public:
    AutoReloadPacket():BoardPacket(){}
    
	uint8_t reload_set;

	virtual void Init(BoardPacketChannelIdType _channelCanId, uint8_t _id)
    {
        BoardPacket::Init(_channelCanId, _id);

        reload_set = 0;
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

class BoardPacketManager : public PacketManager, public CanMsgHandler
{
private:
    StreamChannel m_StreamChannel[BOARD_CHANNEL_NUM];
    CAN_TypeDef *m_pCan;

	FireSetPacket m_fire_set_packet;

	AutoReloadPacket m_auto_reload_packet;

    TestBoardPacket m_testPacket;

    ChassisCtrlMsg m_ctrlMsg;
    ChassisCtrlMsg2 m_ctrlMsg2;
    GimbalRefMsg m_gimbalRefMsg;

protected:
    virtual bool FlushSendBufferLow();

public:
    BoardPacketManager();

    TestBoardPacket& GetTestPacket() { return m_testPacket; }
	FireSetPacket& GetFireSetPacket() { return m_fire_set_packet;}
    ChassisCtrlMsg& GetChassisCtrlMsg() { return m_ctrlMsg; }
    ChassisCtrlMsg2& GetChassisCtrlMsg2() { return m_ctrlMsg2; }
    GimbalRefMsg& GetGimbalRefMsg() { return m_gimbalRefMsg; }
	AutoReloadPacket& GetAutoReload(){return m_auto_reload_packet;}
	
    virtual void Init();
    virtual void HandleNewCanRxMsg(CanRxMsg *_msg);

    static BoardPacketManager *Instance()
    {
        static BoardPacketManager instance;
        return &instance;
    }
};

#endif
