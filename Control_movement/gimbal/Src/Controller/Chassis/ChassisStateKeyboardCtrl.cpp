#include "ChassisStateKeyboardCtrl.hpp"

void ChassisStateKeyboardCtrl::Init(ChassisController* pOwner)
{
	m_IMU = AHRSEstimator::Instance();
	
    m_Vx_filter.SetTau(0.08f);
    m_Vx_filter.SetUpdatePeriod(5);

    m_Vy_filter.SetTau(0.04f);
    m_Vy_filter.SetUpdatePeriod(5);

    m_Vw_filter.SetTau(0.01f);
    m_Vw_filter.SetUpdatePeriod(5);

    chassis_flow_gimbal_pid.kp = 25.0f;
    chassis_flow_gimbal_pid.ki = 0.0f;
    chassis_flow_gimbal_pid.kd = 0.5f;
    chassis_flow_gimbal_pid.maxIOut = 0.0f;
    chassis_flow_gimbal_pid.maxOut = 30.0f;

    m_pGimbalController = (GimbalController*)
        pOwner->GetOwner()->GetEntity(ECT_GimbalController); 

    chassis_flow_gimbal_pid.ref = 0.0f;

    SetMaxMoveVecMag(50.0f);
}

void ChassisStateKeyboardCtrl::Enter(ChassisController* pOwner)
{
	m_Vx_filter.Clear();
    m_Vy_filter.Clear();
    m_Vw_filter.Clear();
}

void ChassisStateKeyboardCtrl::Execute(ChassisController* pOwner)
{
    Dr16* p_dr16 = Dr16::Instance();

    // If no input, XInput and YInput will reset here
	m_XInput = p_dr16->GetLVAxis() * 20.0f;
    m_YInput = -p_dr16->GetLHAxis() * 15.0f;
	
    float shift_rate = 1.0f;

    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_SHIFT,Dr16::KEY_DOWN))
    {
        shift_rate = 1.5f;
    }

    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_W,Dr16::KEY_DOWN))
    {
       m_XInput = 25.0f;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_S,Dr16::KEY_DOWN))
    {
       m_XInput = -25.0f;
    }
    
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_A,Dr16::KEY_DOWN))
    {
       m_YInput = 15.0f;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_D,Dr16::KEY_DOWN))
    {
       m_YInput = -15.0f;
    }

//    float currentMoveVecMagSquare = m_XInput * m_XInput + m_YInput * m_YInput;
//    if(currentMoveVecMagSquare > maxMoveVecMagSquare)
//    {
//        float moveVecMagMultiplier = sqrtf(maxMoveVecMagSquare / currentMoveVecMagSquare);
//        m_XInput *= moveVecMagMultiplier;
//        m_YInput *= moveVecMagMultiplier;
//    }
	
	if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_SHIFT,Dr16::KEY_DOWN))
    {
        m_XInput *= shift_rate;
		m_YInput *= shift_rate;
    }

    absAngle = m_IMU->GetYaw();
	
    relativeAngle = m_pGimbalController->GetGimbalMotor(GimbalController::GMT_Yaw)
        ->pFeedback->positionFdb;

    chassis_flow_gimbal_pid.fdb = -relativeAngle;

    chassis_flow_gimbal_pid.UpdateResult();

    float vxSet = m_XInput * cos(relativeAngle) - m_YInput * sin(relativeAngle);
    float vySet = m_XInput * sin(relativeAngle) + m_YInput * cos(relativeAngle);

    m_WInput = chassis_flow_gimbal_pid.result;

    m_Vx_filter.SetInput(vxSet);
    m_Vx_filter.Update();
    vxSet = m_Vx_filter.GetResult();

    m_Vy_filter.SetInput(vySet);
    m_Vy_filter.Update();
    vySet = m_Vy_filter.GetResult();

    m_Vw_filter.SetInput(m_WInput);
    m_Vw_filter.Update();
    m_WInput = m_Vw_filter.GetResult();

    pOwner->SetVx(vxSet);
    pOwner->SetVy(vySet);
    pOwner->SetVw(m_WInput);
}
void ChassisStateKeyboardCtrl::Exit(ChassisController* pOwner)
{
    ;
}
