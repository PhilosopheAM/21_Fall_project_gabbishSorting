#include "ChassisStateRotate.hpp"

void ChassisStateRotate::Init(ChassisController* pOwner)
{
    m_pGimbalController = (GimbalController*)
        pOwner->GetOwner()->GetEntity(ECT_GimbalController); 
	
	m_Vx_filter.SetTau(0.15f);
    m_Vx_filter.SetUpdatePeriod(5);

    m_Vy_filter.SetTau(0.15f);
    m_Vy_filter.SetUpdatePeriod(5);
}

void ChassisStateRotate::Enter(ChassisController* pOwner)
{
    pOwner->GetCtrlMsg()->SetSpinning(true);
}

void ChassisStateRotate::Execute(ChassisController* pOwner)
{
    m_XInput = Dr16::Instance()->GetLVAxis() * 30.0f;
    m_YInput = -Dr16::Instance()->GetLHAxis() * 30.0f;
    m_WInput = -20.0f;
    
	Dr16* p_dr16 = Dr16::Instance();
	
	if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_W,Dr16::KEY_DOWN))
    {
       m_XInput += 15.0f;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_S,Dr16::KEY_DOWN))
    {
       m_XInput -= 15.0f;
    }
    
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_A,Dr16::KEY_DOWN))
    {
       m_YInput += 9.0f;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_D,Dr16::KEY_DOWN))
    {
       m_YInput -= 9.0f;
    }
	
    relativeAngle = m_pGimbalController->GetGimbalMotor(GimbalController::GMT_Yaw)
      ->pFeedback->positionFdb;
    
    float vxSet = - m_XInput * cos(relativeAngle) + m_YInput * sin(relativeAngle);
    float vySet = m_XInput * sin(relativeAngle) + m_YInput * cos(relativeAngle);
    
//	m_Vx_filter.SetInput(vxSet);
//    m_Vx_filter.Update();
//    vxSet = m_Vx_filter.GetResult();

//    m_Vy_filter.SetInput(vySet);
//    m_Vy_filter.Update();
//    vySet = m_Vy_filter.GetResult();
	
    pOwner->SetVx(vxSet);
    pOwner->SetVy(vySet);
    pOwner->SetVw(m_WInput);
}

void ChassisStateRotate::Exit(ChassisController* pOwner)
{
    pOwner->GetCtrlMsg()->SetSpinning(false);;
}
