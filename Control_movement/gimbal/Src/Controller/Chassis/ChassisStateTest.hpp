#ifndef CHASSISSTATETEST_HPP
#define CHASSISSTATETEST_HPP

#include "StateMachine.hpp"
#include "ChassisController.hpp"
#include "GimbalController.hpp"
#include "math_first_order_filter.h"

class ChassisStateTest : public State<ChassisController>
{
private:
    float m_XInput;
    float m_YInput;
	float m_WInput;

    GimbalController* m_pGimbalController;

    uint32_t m_LastTickHasInput;

    const static float InputThreshold;
    const static uint32_t TickBeforeBreak;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateTest* Instance()
    {
        static ChassisStateTest instance;
        return &instance;
    }
};

#endif
