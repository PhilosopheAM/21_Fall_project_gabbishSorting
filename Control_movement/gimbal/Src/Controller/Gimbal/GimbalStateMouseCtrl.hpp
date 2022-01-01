#ifndef GIMBALSTATEMOUSECTRL_HPP
#define GIMBALSTATEMOUSECTRL_HPP

#include "StateMachine.hpp"
#include "GimbalController.hpp"

class GimbalStateMouseCtrl : public State<GimbalController>
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

    Pid YawPid;
    Pid PitchPid;
public:

    GimbalStateMouseCtrl(){}

    virtual void Init(GimbalController* pOwner);
    virtual void Enter(GimbalController* pOwner);
    virtual void Execute(GimbalController* pOwner);
    virtual void Exit(GimbalController* pOwner);

    static GimbalStateMouseCtrl* Instance()
    {
        static GimbalStateMouseCtrl instance;
        return &instance;
    }
};

#endif
