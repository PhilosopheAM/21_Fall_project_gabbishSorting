#ifndef CHASSISCONTROLLER_HPP
#define CHASSISCONTROLLER_HPP

#include <math.h>
#include "RobotEngine.hpp"
#include "StateMachine.hpp"
#include "Dr16.hpp"
#include "ChassisCtrlMsg.hpp"
#include "ChassisCtrlMsg2.hpp"
#include "BoardPacket.hpp"
#include "AHRSEstimator.hpp"
#include "math_first_order_filter.h"

// Forward declaration
class ChassisController;

class ChassisFsm : public StateMachine<ChassisController>
{
private:
    int ticksBeforeInit;
    bool hasInited;
public:
    ChassisFsm(ChassisController* _pOwner) : StateMachine<ChassisController>(_pOwner){}
    void HandleInput();

    virtual void Init();
};

class ChassisController : public ControllerEntity
{
private:
    float m_Vx;
    float m_Vy;
    float m_Vw;

	FirstOrderFilter m_rotate_filter;

    ChassisCtrlMsg* m_pCtrlMsg;
    ChassisCtrlMsg2* m_pCtrlMsg2;

    AHRSEstimator* m_pIMU;
public:
    ChassisFsm chassisFsm;
	AutoReloadPacket& m_auto_reload;

    ChassisController();

    virtual void Init();
    virtual void Update();

    void Stop()
    {
        m_Vx = 0.0f;
        m_Vy = 0.0f;
        m_Vw = 0.0f;
    }

    void SetVx(float vx){m_Vx = vx;}
    void SetVy(float vy){m_Vy = vy;}
    void SetVw(float vw){m_Vw = vw;}
	
	float GetChassisRotateCompensation();

    float GetVx(){ return m_Vx; }
    float GetVy(){ return m_Vy; }
    float GetVw(){ return m_Vw; }

    ChassisCtrlMsg* GetCtrlMsg(){ return m_pCtrlMsg; }
    ChassisCtrlMsg2* GetCtrlMsg2(){ return m_pCtrlMsg2; }
};

#endif
