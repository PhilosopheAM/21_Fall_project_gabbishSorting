#ifndef CHASSIS_CTRL_MSG_HPP
#define CHASSIS_CTRL_MSG_HPP

#include "CanMsgHandler.hpp"

#define MAX_CHASSIS_CTRL_SPEED 100.0f

enum GimbalFireModeType
{
    GFT_Auto      = 0x00,
    GFT_Burst     = 0x01,
    GFT_SafeAuto  = 0x02,
    GFT_SafeBurst = 0x03
};

enum AimbotStatusType
{
    AST_Manual  = 0x00,
    AST_Aimbot  = 0x01,
    AST_Offline = 0x02
};

class ChassisCtrlMsg : public CanMsgHandler
{
private:
    float m_Vx;
    float m_Vy;
    float m_Vw;

    // Current projectile velcity using by the cannon
    uint8_t m_projectileVelocitySet;
    GimbalFireModeType m_gimbalFireMode;
    AimbotStatusType m_aimbotStatus;
    bool m_isAmmoLidOpen;
    bool m_isSpinning;
    bool m_isLowFireRate;
    bool m_isFricOn;

    static const uint8_t kFricMask;
    static const uint8_t kSpinningMask;
    static const uint8_t kAmmoLidMask;
    static const uint8_t kFireRateMask;
    static const uint8_t kAstOffset;
    static const uint8_t kGftOffset;
    static const uint8_t kEnumMask;

    CAN_TypeDef* m_pCan;
    uint32_t m_canId;

    float UncompressUint16(uint16_t _in);
    uint16_t CompressFloat(float _in);
public:
    float GetVx() const { return m_Vx; }
    float GetVy() const { return m_Vy; }
    float GetVw() const { return m_Vw; }
    void SetVx(float _vx) { m_Vx = _vx; }
    void SetVy(float _vy) { m_Vy = _vy; }
    void SetVw(float _vw) { m_Vw = _vw; }

    uint8_t GetProjectileVelocity() const { return m_projectileVelocitySet; }
    GimbalFireModeType GetGimbalFireMode() const { return m_gimbalFireMode; }
    AimbotStatusType GetAimbotStatus() const { return m_aimbotStatus; }
    bool IsAmmoLidOpen() const { return m_isAmmoLidOpen; }
    bool IsSpinning() const { return m_isSpinning; }
    bool IsLowFireRate() const { return m_isLowFireRate; }
    bool IsFricOn() const { return m_isFricOn; }

    void SetProjectileVelocity(uint8_t velocity) { m_projectileVelocitySet = velocity; }
    void SetGimbalFireMode(GimbalFireModeType gft) { m_gimbalFireMode = gft; }
    void SetAimbotStatus(AimbotStatusType ast) { m_aimbotStatus = ast; }
    void SetAmmoLidOpen(bool isOpen) { m_isAmmoLidOpen = isOpen; }
    void SetSpinning(bool isSpinning) { m_isSpinning = isSpinning; }
    void SetLowFireRate(bool lowFireRate) { m_isLowFireRate = lowFireRate; }
    void SetFricOn(bool fricOn) { m_isFricOn = fricOn; }


    void Init(CAN_TypeDef* can, uint32_t _stdId);
    void SendMsg();

    virtual void HandleNewCanRxMsg(CanRxMsg* _msg);
};

#endif
