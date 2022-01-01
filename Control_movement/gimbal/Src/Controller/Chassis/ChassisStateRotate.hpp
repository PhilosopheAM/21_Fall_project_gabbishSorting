#ifndef CHASSISSTATEROTATE_HPP
#define CHASSISSTATEROTATE_HPP

#include "StateMachine.hpp"
#include "ChassisController.hpp"
#include "GimbalController.hpp"
#include "math_first_order_filter.h"

class ChassisStateRotate : public State<ChassisController>
{
private:
    float m_XInput;
    float m_YInput;
	float m_WInput;

	float relativeAngle;
    GimbalController* m_pGimbalController;

    uint32_t m_LastTickHasInput;

	FirstOrderFilter m_Vx_filter;
    FirstOrderFilter m_Vy_filter;

    const static float InputThreshold;
    const static uint32_t TickBeforeBreak;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateRotate* Instance()
    {
        static ChassisStateRotate instance;
        return &instance;
    }
};

#endif
