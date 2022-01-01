#ifndef GIMBAL_REF_MSG_HPP
#define GIMBAL_REF_MSG_HPP

#include "CanMsgHandler.hpp"
#include "JudgeSystem.hpp"

class GimbalRefMsg : public CanMsgHandler
{
private:
    uint16_t m_heatRemainning;
    uint16_t m_maxProjectileVelocity;
    uint8_t m_currentTeam;

    JudgeSystem* m_pJudgeSystem;

    CAN_TypeDef* m_pCan;
    uint32_t m_canId;
public:
    void Init(CAN_TypeDef* can, uint32_t _stdId);
    void SendMsg();

    virtual void HandleNewCanRxMsg(CanRxMsg* _msg);

    uint16_t GetHeatRemainning() const { return m_heatRemainning; }
    uint16_t GetMaxProjectileVelocity() const { return m_maxProjectileVelocity; }
};

#endif
