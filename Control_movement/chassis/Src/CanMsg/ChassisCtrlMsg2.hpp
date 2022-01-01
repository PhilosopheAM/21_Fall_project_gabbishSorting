#ifndef CHASSIS_CTRL_MSG_2_HPP
#define CHASSIS_CTRL_MSG_2_HPP

#include "CanMsgHandler.hpp"

class ChassisCtrlMsg2 : public CanMsgHandler
{
private:
    float m_pitchAngle;
    uint8_t m_useSCap;

    CAN_TypeDef* m_pCan;
    uint32_t m_canId;
public:
    float GetPitchAngle() const { return m_pitchAngle; }
    bool UseSCap() const { return m_useSCap != 0x00; }
    void SetPitchAngle(float pitch) { m_pitchAngle = pitch; }
    void SetUseSCap(bool useSCap) { m_useSCap = useSCap ? 0x01 : 0x00; }

    void Init(CAN_TypeDef* can, uint32_t _stdId);
    void SendMsg();

    virtual void HandleNewCanRxMsg(CanRxMsg* _msg);
};

#endif
