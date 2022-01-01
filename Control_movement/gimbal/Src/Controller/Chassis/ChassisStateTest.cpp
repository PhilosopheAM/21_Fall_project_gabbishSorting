#include "ChassisStateTest.hpp"

void ChassisStateTest::Init(ChassisController* pOwner)
{
	m_pGimbalController = (GimbalController*)
    pOwner->GetOwner()->GetEntity(ECT_GimbalController); 
}

void ChassisStateTest::Enter(ChassisController* pOwner)
{
   ;
}

void ChassisStateTest::Execute(ChassisController* pOwner)
{
	Dr16* p_dr16 = Dr16::Instance();

	m_XInput = p_dr16->GetLVAxis() * 10.0f;
   	m_YInput = - p_dr16->GetLHAxis() * 10.0f;
	m_WInput = - p_dr16->GetRHAxis() * 15.0f;
	
	
	if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_W, Dr16::KEY_DOWN))
    {
       m_XInput += 9.0f;
    }
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_S, Dr16::KEY_DOWN))
    {
       m_XInput -= 9.0f;
    }
    
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_A, Dr16::KEY_DOWN))
    {
       m_YInput += 9.0f;
    }
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_D, Dr16::KEY_DOWN))
    {
       m_YInput -= 9.0f;
    }

    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_Q, Dr16::KEY_DOWN))
    {
       m_WInput += 9.0f;
    }
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_E, Dr16::KEY_DOWN))
    {
       m_WInput -= 9.0f;
    }
	
    float relativeAngle = -m_pGimbalController->GetYawRelativeAngleRad();

    float vxSet = m_XInput * cos(relativeAngle) + m_YInput * sin(relativeAngle);
    float vySet = - m_XInput * sin(relativeAngle) + m_YInput * cos(relativeAngle);

	// m_WInput+= -Dr16::Instance()->GetMouseX() * 0.1f;
	
	pOwner->SetVx(vxSet);
   	pOwner->SetVy(vySet);
	pOwner->SetVw(m_WInput);
}

void ChassisStateTest::Exit(ChassisController* pOwner)
{
    ;
}
