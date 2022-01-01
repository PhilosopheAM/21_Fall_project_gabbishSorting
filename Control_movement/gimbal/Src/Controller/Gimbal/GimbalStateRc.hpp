#ifndef GIMBALSTATERC_HPP
#define GIMBALSTATERC_HPP

#include "StateMachine.hpp"
#include "GimbalController.hpp"

//#include "Observer.hpp

class GimbalStateRc : public State<GimbalController>
{
private:
    float m_YawSet;
    float m_PitchSet;

    float m_MaxYaw;
    float m_MinYaw;
    
    float m_MaxPitch;
    float m_MinPitch;

    G6020* m_pYawMotor;
    G6020* m_pPitchMotor;

public:

    GimbalStateRc(){}

    virtual void Init(GimbalController* pOwner);
    virtual void Enter(GimbalController* pOwner);
    virtual void Execute(GimbalController* pOwner);
    virtual void Exit(GimbalController* pOwner);

    static GimbalStateRc* Instance()
    {
        static GimbalStateRc instance;
        return &instance;
    }
};

#endif
