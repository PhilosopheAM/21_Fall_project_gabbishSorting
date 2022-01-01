#ifndef GIMBALCONTROLLER_HPP
#define GIMBALCONTROLLER_HPP

#include <math.h>
#include "RobotEngine.hpp"
#include "StateMachine.hpp"
#include "Dr16.hpp"
#include "G6020.hpp"
#include "HostCom.hpp"
#include "BoardPacket.hpp"

// Forward declaration
class GimbalController;

class GimbalFsm : public StateMachine<GimbalController>
{
private:
    int ticksBeforeInit;
    bool hasInited;
public:
    GimbalFsm(GimbalController* _pOwner) : StateMachine<GimbalController>(_pOwner){}
    void HandleInput();

    virtual void Init();
};

class GimbalController : public ControllerEntity
{
private:
    G6020 m_GimbalMotor[2];
    bool m_UseImu;

    const static float G6020_YAW_SPD_KP;
    const static float G6020_YAW_SPD_KI;
    const static float G6020_YAW_SPD_KD;
    const static float G6020_YAW_SPD_MAXOUT;
    const static float G6020_YAW_SPD_IMAXOUT;

    const static float G6020_YAW_POS_KP;
    const static float G6020_YAW_POS_KI;
    const static float G6020_YAW_POS_KD;
    const static float G6020_YAW_POS_MAXOUT;
    const static float G6020_YAW_POS_IMAXOUT;

    const static float G6020_PITCH_SPD_KP;
    const static float G6020_PITCH_SPD_KI;
    const static float G6020_PITCH_SPD_KD;
    const static float G6020_PITCH_SPD_MAXOUT;
    const static float G6020_PITCH_SPD_IMAXOUT;

    const static float G6020_PITCH_POS_KP;
    const static float G6020_PITCH_POS_KI;
    const static float G6020_PITCH_POS_KD;
    const static float G6020_PITCH_POS_MAXOUT;
    const static float G6020_PITCH_POS_IMAXOUT;
public:
    enum GimbalMotorType
    {
        GMT_Yaw = 0,
        GMT_Pitch,

        GMT_LENGTH
    };

    GimbalFsm gimbalFsm;

	HostCom* p_HostCom;

    float zero_point;
		
    GimbalController();

    virtual void Init();
    virtual void Update();

    void GimbalUseImu(bool _state);

    float GetYawRelativeAngleRad();

    G6020* GetGimbalMotor(GimbalMotorType _motor){ return &m_GimbalMotor[_motor]; }
};

#endif
