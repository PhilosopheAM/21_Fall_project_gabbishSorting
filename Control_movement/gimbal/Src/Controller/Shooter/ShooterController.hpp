#ifndef SHOOTERCONTROLLER_HPP
#define SHOOTERCONTROLLER_HPP

#include <math.h>
#include "RobotEngine.hpp"
#include "StateMachine.hpp"
#include "Dr16.hpp"
#include "M2006.hpp"
#include "BoardPacket.hpp"
#include "HostPacket.hpp"
#include "bsp_fric.h"

#include "ChassisCtrlMsg.hpp"
#include "GimbalRefMsg.hpp"

// Forward declaration
class ShooterController;

class ShooterController : public ControllerEntity
{
private:
    ChassisCtrlMsg* m_pCtrlMsg;
    GimbalRefMsg* m_pRefMsg;

    GimbalFireModeType m_currentFireMode;
    bool m_enteringBurstMode;

    const static uint16_t kHeatPerProjectile;
    const static uint16_t kNumberOfSafeShots;
    const static uint16_t kBurstNumber;
    const static float kNumberOfTriggerFork;
public:
	float trigger_spd_set;
	uint8_t fire_state_set;
	uint16_t shoot_spd_set;

    float fullAutoTriggerSpd;
    int ticksBeforeFire;
    int fircOnFlag;
    bool ammoLidIsOpen;

    M2006 triggerMotor;

	FireSetPacket& fireSetPacket;
	//FireFdbPacket& fireFdbPacket;

    ShooterController();

    virtual void Init();
    virtual void Update();

    GimbalFireModeType GetCurrentFireMode() const { return m_currentFireMode; }
};

#endif
