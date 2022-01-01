#ifndef GIMBALSTATEMOUSECONTROL_HPP
#define GIMBALSTATEMOUSECONTROL_HPP

#include "StateMachine.hpp"
#include "GimbalController.hpp"
#include "ChassisController.hpp"
#include "AHRSEstimator.hpp"
#include "math_first_order_filter.h"
#include "HostCom.hpp"
#include "Pid.hpp"

class GimbalStateMouseControl : public State<GimbalController>
{
private:
    float m_YawSet;
    float m_PitchSet;

    float m_MaxYaw;
    float m_MinYaw;
    
    float m_MaxPitch;
    float m_MinPitch;

	float m_LastYawErr;
	float m_LastPitchErr;
	uint32_t lastPacketTime;

    float yawSumErr;
    float aimbotOffset;
    float offsetSensitivity;

    G6020* m_pYawMotor;
    G6020* m_pPitchMotor;

	FirstOrderFilter yaw_filter;
	FirstOrderFilter yaw_tag_filter;
	FirstOrderFilter pitch_tag_filter;

	HostCom* host_com;
    AHRSEstimator* m_IMU;
	ChassisController* m_chassis;

    Pid YawPid;
    Pid PitchPid;
public:
    GimbalStateMouseControl(){}

    virtual void Init(GimbalController* pOwner);
    virtual void Enter(GimbalController* pOwner);
    virtual void Execute(GimbalController* pOwner);
    virtual void Exit(GimbalController* pOwner);

    static GimbalStateMouseControl* Instance()
    {
        static GimbalStateMouseControl instance;
        return &instance;
    }
};

#endif
