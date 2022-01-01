#include "ChassisHeadingState.hpp"
#include "Math.hpp"

void ChassisHeadingState::Init(ChassisController* pOwner)
{
	m_pGimbalController = (GimbalController*)pOwner->GetOwner()->GetEntity(ECT_GimbalController); 

	m_angularPid.kp = 15.0f;
    m_angularPid.ki = 1.0f;
    m_angularPid.kd = 0.5f;
    m_angularPid.maxIOut = 0.0f;
    m_angularPid.maxOut = 30.0f;

	m_angularPid.ref = 0.0f;

    m_Vx_filter.SetTau(0.15f);
    m_Vx_filter.SetUpdatePeriod(5);

    m_Vy_filter.SetTau(0.15f);
    m_Vy_filter.SetUpdatePeriod(5);

    m_Vw_filter.SetTau(0.15f);
    m_Vw_filter.SetUpdatePeriod(5);

	SetMaxMoveVecMag(15.0f);
}

void ChassisHeadingState::Enter(ChassisController* pOwner)
{
   	m_Vx_filter.Clear();
    m_Vy_filter.Clear();
    m_Vw_filter.Clear();
}

void ChassisHeadingState::Execute(ChassisController* pOwner)
{
    Dr16* p_dr16 = Dr16::Instance();

    // If no input, XInput and YInput will reset here
	float XInput = p_dr16->GetLVAxis() * 35.0f;
    float YInput = -p_dr16->GetLHAxis() * 35.0f;
	
    float shift_rate = 1.0f;

    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_SHIFT,Dr16::KEY_DOWN))
    {
        shift_rate = 1.5f;
    }

    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_W,Dr16::KEY_DOWN))
    {
       XInput += maxMoveVecMag;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_S,Dr16::KEY_DOWN))
    {
       XInput -= maxMoveVecMag;
    }
    
    if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_A,Dr16::KEY_DOWN))
    {
       YInput += maxMoveVecMag;
    }
    else if(p_dr16->QueryPcKeyState(Dr16::PC_KEY_D,Dr16::KEY_DOWN))
    {
       YInput -= maxMoveVecMag;
    }

    float currentMoveVecMagSquare = XInput * XInput + YInput * YInput;

	if(currentMoveVecMagSquare < 0.01f)
	{
		pOwner->Stop();
        m_Vx_filter.Clear();
        m_Vy_filter.Clear();
        m_Vw_filter.Clear();
		return;
	}

    if(currentMoveVecMagSquare > maxMoveVecMagSquare)
    {
        float moveVecMagMultiplier = sqrtf(maxMoveVecMagSquare / currentMoveVecMagSquare);
        XInput *= moveVecMagMultiplier;
        YInput *= moveVecMagMultiplier;
    }

    XInput *= shift_rate;
    YInput *= shift_rate;
	
	float targetAngle = atan2f(YInput, XInput);
    float relativeAngle = -m_pGimbalController->GetYawRelativeAngleRad();

	float relativeErr = Math::LoopFloatConstrain(abs(targetAngle - relativeAngle), 0.0f, Math::Pi);
	float realTargetAngle = relativeErr < (0.5f * Math::Pi) ? targetAngle : (targetAngle - Math::Pi);
	realTargetAngle = Math::LoopFloatConstrain(realTargetAngle, -Math::Pi, Math::Pi);

	m_angularPid.ref = realTargetAngle;
	m_angularPid.fdb = relativeAngle;
	m_angularPid.UpdateResult();
	float vwSet = m_angularPid.result;

    float vxSet = XInput * cos(relativeAngle) + YInput * sin(relativeAngle);
    float vySet = - XInput * sin(relativeAngle) + YInput * cos(relativeAngle);
	
	pOwner->SetVx(vxSet);
   	pOwner->SetVy(vySet);
	pOwner->SetVw(vwSet);
}

void ChassisHeadingState::Exit(ChassisController* pOwner)
{
    ;
}
