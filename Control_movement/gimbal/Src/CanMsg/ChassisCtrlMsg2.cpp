#include "ChassisCtrlMsg2.hpp"
#include "CanMsgDispatcher.hpp"
#include "Stream.hpp"

void ChassisCtrlMsg2::HandleNewCanRxMsg(CanRxMsg* _msg)
{
    CanMsgHandler::HandleNewCanRxMsg(_msg);

    uint8_t* ptr = lastCanMsg.Data;

    m_pitchAngle = ArrayStreamHelper::ReadFloat(ptr);
    m_useSCap = ArrayStreamHelper::ReadUint8(ptr);
}

void ChassisCtrlMsg2::Init(CAN_TypeDef* can, uint32_t _stdId)
{
    m_canId = _stdId;
    m_pCan = can;
    CanMsgDispatcher::Instance()->RegisterHandler(m_pCan, m_canId, this);
}

void ChassisCtrlMsg2::SendMsg()
{
    uint8_t buffer[8] = {0};

    uint8_t* ptr = buffer;

    ArrayStreamHelper::Write(ptr, m_pitchAngle);
    ArrayStreamHelper::Write(ptr, m_useSCap);

    CanManager::Instance()->CanTransmit(m_pCan, m_canId, buffer, sizeof(buffer));
}
