#include "ShooterController.hpp"
#include "bsp_laser.h"
#include <cmath>
 
const uint16_t ShooterController::kHeatPerProjectile = 10;
const uint16_t ShooterController::kNumberOfSafeShots = 5;

const uint16_t ShooterController::kBurstNumber = 3;
const float ShooterController::kNumberOfTriggerFork = 8.0f;

ShooterController::ShooterController() : ControllerEntity(ECT_ShooterController),fireSetPacket(BoardPacketManager::Instance()->GetFireSetPacket())
{
	
}

void ShooterController::Init()
{
    SetDefaultTicksToUpdate(1);
	bsp_laser_init();
	bsp_laser_on();
	
	fricInit();
	
    triggerMotor.RegistMotor(CAN1, 0x203);
    triggerMotor.controlMode = Motor::SPD_MODE;
	triggerMotor.sensorFeedBack.offset = 0x0000;//TODO
    triggerMotor.pidSpeed.kp = 3000;
    triggerMotor.pidSpeed.ki = 0;
    triggerMotor.pidSpeed.kd = 1;
    triggerMotor.pidSpeed.maxOut = 30000;
    triggerMotor.pidSpeed.maxIOut = 0;

	triggerMotor.pidPosition.kp = 15.0f;
	triggerMotor.pidPosition.ki = 0.0f;
	triggerMotor.pidPosition.kd = 0.0f;
	triggerMotor.pidPosition.maxOut = 5.0f;
	triggerMotor.pidPosition.maxIOut = 0.0f;

	fullAutoTriggerSpd = -4.0f;
    ticksBeforeFire = 500;
    fircOnFlag = 0;
    ammoLidIsOpen = false;

	m_currentFireMode = GFT_Auto;
	m_enteringBurstMode = true;

    m_pCtrlMsg = &BoardPacketManager::Instance()->GetChassisCtrlMsg();
	m_pRefMsg = &BoardPacketManager::Instance()->GetGimbalRefMsg();
}

void ShooterController::Update()
{
	Dr16* pDr16 = Dr16::Instance();

	uint16_t maxProjectileVelocity = m_pRefMsg->GetMaxProjectileVelocity();
	uint16_t projectielVelocitySet = maxProjectileVelocity;

	// Low fire rate.
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_Z, Dr16::KEY_DOWN))
	{
		fullAutoTriggerSpd = -4.0f;
		m_pCtrlMsg->SetLowFireRate(true);
	}
    
	// High fire rate.
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_X, Dr16::KEY_DOWN))
    {
		fullAutoTriggerSpd = -8.0f;
		m_pCtrlMsg->SetLowFireRate(false);
    }

	// NOTE: The timing trick will only work when the entity's default update tick is 1.
	// Toggle heat control.
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_F, Dr16::KEY_FALL)
		&& Time::GetTick() == pDr16->GetLastUpdateTick())
	{
		switch (m_currentFireMode)
		{
		case GFT_Auto:
			m_currentFireMode = GFT_SafeAuto;
			break;
		case GFT_Burst:
			m_currentFireMode = GFT_SafeBurst;
			break;
		case GFT_SafeAuto:
			m_currentFireMode = GFT_Auto;
			break;
		case GFT_SafeBurst:
			m_currentFireMode = GFT_Burst;
			break;
		default:
			break;
		}
	}

	// NOTE: The timing trick will only work when the entity's default update tick is 1.
	// Toggle firemode
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_C, Dr16::KEY_FALL)
		&& Time::GetTick() == pDr16->GetLastUpdateTick())
	{
		switch (m_currentFireMode)
		{
		case GFT_Auto:
			m_currentFireMode = GFT_Burst;
			m_enteringBurstMode = true;
			break;
		case GFT_Burst:
			m_currentFireMode = GFT_Auto;
			break;
		case GFT_SafeAuto:
			m_currentFireMode = GFT_SafeBurst;
			m_enteringBurstMode = true;
			break;
		case GFT_SafeBurst:
			m_currentFireMode = GFT_SafeAuto;
			break;
		default:
			break;
		}
	}
	
	// Warming up fric.
	if(pDr16->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_M2D))
    {
		fircOnFlag = 1;
    }
	
	// STAHP attacking!
	if (pDr16->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_D2M)||
		pDr16->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_U2M))
    {
		fircOnFlag = 0;
		triggerMotor.speedSet = 0.0f;
    }

	// Hold R to revserse the trigger motor, anti-congest measure
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_R, Dr16::KEY_DOWN))
    {
		triggerMotor.speedSet = 2.0f;
    }
	else
	{
		triggerMotor.speedSet = 0.0f;
	}

	if(m_currentFireMode == GFT_Burst || m_currentFireMode == GFT_SafeBurst)
	{
		if(m_enteringBurstMode)
		{
			m_enteringBurstMode = false;
			triggerMotor.positionSet = triggerMotor.sensorFeedBack.positionFdb;
		}
		triggerMotor.controlMode = Motor::POS_MODE;

		// NOTE: The timing trick will only work when the entity's default update tick is 1.
		if(pDr16->QueryPcKeyState(Dr16::PC_M_L, Dr16::KEY_FALL)
			&& Time::GetTick() == pDr16->GetLastUpdateTick())
		{
			fircOnFlag = 1;
			triggerMotor.positionSet = triggerMotor.sensorFeedBack.positionFdb - Math::PiX2 / kNumberOfTriggerFork * kBurstNumber;
		}
	}

	if(m_currentFireMode == GFT_SafeAuto || m_currentFireMode == GFT_Auto)
	{
		m_enteringBurstMode = true;
		triggerMotor.controlMode = Motor::SPD_MODE;
  
		// Mouse 1 to fire, higher pirority than anti-congest
    	if(pDr16->QueryPcKeyState(Dr16::PC_M_L, Dr16::KEY_DOWN))
    	{
			fircOnFlag = 1;

			triggerMotor.speedSet = fullAutoTriggerSpd;
   		}
    
		// Release mouse 1
    	if(pDr16->QueryPcKeyState(Dr16::PC_M_L, Dr16::KEY_RISE))
		{
        	triggerMotor.speedSet = 0.0f;
    	}
	}

	// Heat control
	if(m_currentFireMode == GFT_SafeAuto || m_currentFireMode == GFT_SafeBurst)
	{
		if(kNumberOfSafeShots * kHeatPerProjectile > m_pRefMsg->GetHeatRemainning())
		{
			// Stop for AUTO mode.
			triggerMotor.speedSet = 0.0f;
			// Stop for BURST mode.
			triggerMotor.positionSet = triggerMotor.sensorFeedBack.positionFdb;
		}
	}
	
	// Set fric speed for fire control module, set fricOnFlag BEFORE section below.
	if(fircOnFlag == 1)
	{
		fire_state_set = 1;
		shoot_spd_set = 3270;
		m_pCtrlMsg->SetFricOn(true);
	}
	else
	{
		fire_state_set = 0;
		m_pCtrlMsg->SetFricOn(false);
	}
	
	// Heart beat packet to fire control module 
    if(Time::GetTick() % 10 == 0)
    {
		fireSetPacket.speed_set = shoot_spd_set;
		fireSetPacket.fire_state_set = fire_state_set;
		fireSetPacket.SendPacket();
    }

	// NOTE: The timing trick will only work when the entity's default update tick is 1.
	if(pDr16->QueryPcKeyState(Dr16::PC_KEY_G, Dr16::KEY_FALL)
		&& Time::GetTick() == pDr16->GetLastUpdateTick())
    {
		if(!ammoLidIsOpen)
		{
			TIM1->CCR4 = 2500;
			ammoLidIsOpen = true;
			m_pCtrlMsg->SetAmmoLidOpen(true);
		}else{
			TIM1->CCR4 = 500;
			ammoLidIsOpen = false;
			m_pCtrlMsg->SetAmmoLidOpen(false);
		}
    }

	m_pCtrlMsg->SetGimbalFireMode(m_currentFireMode);
	m_pCtrlMsg->SetProjectileVelocity(projectielVelocitySet);
}
