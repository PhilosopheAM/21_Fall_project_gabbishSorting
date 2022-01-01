#ifndef CHASSIS_HEADING_STATE_HPP
#define CHASSIS_HEADING_STATE_HPP

#include "StateMachine.hpp"
#include "ChassisController.hpp"
#include "GimbalController.hpp"
#include "Pid.hpp"
#include "math_first_order_filter.h"

// Make the chassis always head to the most recent input vector
class ChassisHeadingState : public State<ChassisController>
{
private:
    // Max movement vector magnitude
    float maxMoveVecMag;
    float maxMoveVecMagSquare;

    GimbalController* m_pGimbalController;
    FirstOrderFilter m_Vx_filter;
    FirstOrderFilter m_Vy_filter;
    FirstOrderFilter m_Vw_filter;

    Pid m_angularPid;
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

    static ChassisHeadingState* Instance()
    {
        static ChassisHeadingState instance;
        return &instance;
    }
};

#endif
