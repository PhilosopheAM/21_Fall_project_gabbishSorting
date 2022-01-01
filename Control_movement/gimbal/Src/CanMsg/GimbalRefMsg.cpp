#include "GimbalRefMsg.hpp"
#include "CanMsgDispatcher.hpp"
#include "Stream.hpp"

void GimbalRefMsg::HandleNewCanRxMsg(CanRxMsg* _msg)
{
    CanMsgHandler::HandleNewCanRxMsg(_msg);

    uint8_t* ptr = lastCanMsg.Data;

    m_heatRemainning = ArrayStreamHelper::ReadUint16(ptr);
    m_maxProjectileVelocity = ArrayStreamHelper::ReadUint16(ptr);
    m_currentTeam = ArrayStreamHelper::ReadUint8(ptr);
}

void GimbalRefMsg::Init(CAN_TypeDef* can, uint32_t _stdId)
{
    m_heatRemainning = 0xFF;
    m_maxProjectileVelocity = 12;

    m_pJudgeSystem = JudgeSystem::Instance();

    m_canId = _stdId;
    m_pCan = can;
    CanMsgDispatcher::Instance()->RegisterHandler(m_pCan, m_canId, this);
}

void GimbalRefMsg::SendMsg()
{
    uint16_t maxHeat = m_pJudgeSystem->GameRobotStatus.shooter_id2_17mm_cooling_limit;
    uint16_t currentHeat = m_pJudgeSystem->PowerHeatData.shoot_id2_17mm_cooling_heat;
    m_heatRemainning = maxHeat > currentHeat ? maxHeat - currentHeat : 0;

    m_maxProjectileVelocity = m_pJudgeSystem->GameRobotStatus.shooter_id2_17mm_speed_limit;

    // Red: 1 2 3 4 5 6 7 8 9
    // Blu：101 102 103 104 105 106 107 108 109
    // m_currentTeam will be 0 if in red team, 
    if(m_pJudgeSystem->GameRobotStatus.robot_id == 0)
    {
        // Error, no connection to judge system
        m_currentTeam = 255;
    }
    else
    {
        m_currentTeam = m_pJudgeSystem->GameRobotStatus.robot_id < 50 ? 0 : 1;
    }


    uint8_t buffer[8] = {0};

    uint8_t* ptr = buffer;

    ArrayStreamHelper::Write(ptr, m_heatRemainning);
    ArrayStreamHelper::Write(ptr, m_maxProjectileVelocity);
    ArrayStreamHelper::Write(ptr, m_currentTeam);

    CanManager::Instance()->CanTransmit(m_pCan, m_canId, buffer, sizeof(buffer));
}

