#ifndef CHASSISSTATEKEYBOARDCTRL_HPP
#define CHASSISSTATEKEYBOARDCTRL_HPP

#include "StateMachine.hpp"
#include "ChassisController.hpp"
#include "GimbalController.hpp"
#include "AHRSEstimator.hpp"
#include "Pid.hpp"
#include "Dr16.hpp"
#include "math_first_order_filter.h"

class ChassisStateKeyboardCtrl : public State<ChassisController>
{
private:
    float m_XInput;
    float m_YInput;
    float m_WInput;

    float relativeAngle;
    float absAngle;

    // Max movement vector magnitude
    float maxMoveVecMag;
    float maxMoveVecMagSquare;

    FirstOrderFilter m_Vx_filter;
    FirstOrderFilter m_Vy_filter;
    FirstOrderFilter m_Vw_filter;
 
    GimbalController* m_pGimbalController;
    AHRSEstimator* m_IMU;
    Pid chassis_flow_gimbal_pid;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    float GetMaxMoveVecMag() const { return maxMoveVecMag; }
    void SetMaxMoveVecMag(float _maxMag)
    {
        maxMoveVecMag = _maxMag;
        maxMoveVecMagSquare = _maxMag * _maxMag;
    }

    static ChassisStateKeyboardCtrl* Instance()
    {
        static ChassisStateKeyboardCtrl instance;
        return &instance;
    }
};

#endif
