#include "ChassisController.hpp"
#include "ChassisStateTest.hpp"
#include "ChassisStateKeyboardCtrl.hpp"
#include "ChassisStateRotate.hpp"
#include "ChassisHeadingState.hpp"

ChassisController::ChassisController() : ControllerEntity(ECT_ChassisController)
    ,chassisFsm(this),m_auto_reload(BoardPacketManager::Instance()->GetAutoReload())
    {}

void ChassisController::Init()
{
    SetDefaultTicksToUpdate(5);
	
    chassisFsm.Init();

    m_pCtrlMsg = &BoardPacketManager::Instance()->GetChassisCtrlMsg();
    m_pCtrlMsg2 = &BoardPacketManager::Instance()->GetChassisCtrlMsg2();

    m_pIMU = AHRSEstimator::Instance();
	
	m_rotate_filter.SetUpdatePeriod(5);
	m_rotate_filter.SetTau(0.1f);
}

void ChassisController::Update()
{
    chassisFsm.HandleInput();
    chassisFsm.Update();

	m_pCtrlMsg->SetVx(m_Vx);
    m_pCtrlMsg->SetVy(m_Vy);
    m_pCtrlMsg->SetVw(m_Vw);

	m_rotate_filter.SetInput(m_Vw);
	m_rotate_filter.Update();
	
	m_pCtrlMsg->SendMsg();

    // rad2deg
    float gimbalPitch = m_pIMU->GetRoll() * 57.295779513082320876798154814105f;
    m_pCtrlMsg2->SetPitchAngle(gimbalPitch);

    bool useSCap = Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_SHIFT, Dr16::KEY_DOWN);
    m_pCtrlMsg2->SetUseSCap(useSCap);

    m_pCtrlMsg2->SendMsg();
}

float ChassisController::GetChassisRotateCompensation()
{
	return m_rotate_filter.GetResult();
}

void ChassisFsm::HandleInput()
{
    // Cache the pointer for less function call.
    Dr16* pDr16 = Dr16::Instance();
	if(!hasInited)
    {
        if(Time::GetTick() > ticksBeforeInit)
        {
            ChangeState(ChassisStateKeyboardCtrl::Instance());
            hasInited = true;
        }
    }

	if(pDr16->QuerySwState(Dr16::RC_SW_L, Dr16::RC_SW_DOWN))
	{
		ChangeState(ChassisStateTest::Instance());
        return;
	}
	
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_CTRL, Dr16::KEY_DOWN))
    {
		if(pDr16->QueryPcKeyState(Dr16::PC_KEY_Q, Dr16::KEY_FALL))
		{
			m_pOwner->m_auto_reload.reload_set = 1;
			m_pOwner->m_auto_reload.SendPacket();
		}
		else if(pDr16->QueryPcKeyState(Dr16::PC_KEY_E, Dr16::KEY_FALL))
		{
			m_pOwner->m_auto_reload.reload_set = 2;
			m_pOwner->m_auto_reload.SendPacket();
		}
    }
	
	if(pDr16->QuerySwState(Dr16::RC_SW_L, Dr16::RC_SW_MID))
	{
        if(pDr16->QueryPcKeyState(Dr16::PC_KEY_V, Dr16::KEY_DOWN))
        {
		    ChangeState(ChassisStateRotate::Instance());
            return;
        }
        
        if(pDr16->QueryPcKeyState(Dr16::PC_KEY_B, Dr16::KEY_DOWN))
        {
		    ChangeState(ChassisHeadingState::Instance());
            return;
        }

        ChangeState(ChassisStateKeyboardCtrl::Instance());
    }
}
	
    

void ChassisFsm::Init()
{
    hasInited = false;
    ticksBeforeInit = 250;

    ChassisStateKeyboardCtrl::Instance()->Init(m_pOwner);
	ChassisStateRotate::Instance()->Init(m_pOwner);
	ChassisStateTest::Instance()->Init(m_pOwner);
    ChassisHeadingState::Instance()->Init(m_pOwner);
    SetCurrentState(ChassisStateTest::Instance());
}
