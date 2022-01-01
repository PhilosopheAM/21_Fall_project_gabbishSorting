#include "GimbalController.hpp"
#include "GimbalStateRc.hpp"
#include "GimbalStateMouseControl.hpp"
#include "AHRSEstimator.hpp"

#include "Led.hpp"

GimbalController::GimbalController() : ControllerEntity(ECT_GimbalController),
    m_UseImu(false),
    gimbalFsm(this)
{}
	
const float GimbalController::G6020_YAW_SPD_KP = 3300.0f;
const float GimbalController::G6020_YAW_SPD_KI = 0.0f;
const float GimbalController::G6020_YAW_SPD_KD = 3.0f;
const float GimbalController::G6020_YAW_SPD_MAXOUT = 30000.0f;
const float GimbalController::G6020_YAW_SPD_IMAXOUT = 30000.0f;

const float GimbalController::G6020_YAW_POS_KP = 100.0f;
const float GimbalController::G6020_YAW_POS_KI = 0.0f; 
const float GimbalController::G6020_YAW_POS_KD = 2.0f;
const float GimbalController::G6020_YAW_POS_MAXOUT = 400.0f;
const float GimbalController::G6020_YAW_POS_IMAXOUT = 10.0f;

const float GimbalController::G6020_PITCH_SPD_KP = 3300.0f;
const float GimbalController::G6020_PITCH_SPD_KI = 0.0f;
const float GimbalController::G6020_PITCH_SPD_KD = 15.0f;
const float GimbalController::G6020_PITCH_SPD_MAXOUT = 30000.0f;
const float GimbalController::G6020_PITCH_SPD_IMAXOUT = 30000.0f;
    
const float GimbalController::G6020_PITCH_POS_KP = 35.0f;
const float GimbalController::G6020_PITCH_POS_KI = 0.0f;
const float GimbalController::G6020_PITCH_POS_KD = 0.0f;
const float GimbalController::G6020_PITCH_POS_MAXOUT = 350.0f;
const float GimbalController::G6020_PITCH_POS_IMAXOUT = 10.0f;

void GimbalController::Init()
{
    SetDefaultTicksToUpdate(5);

    m_GimbalMotor[GMT_Yaw].RegistMotor(CAN1, 0x206);
    m_GimbalMotor[GMT_Yaw].controlMode = Motor::POS_MODE;
    m_GimbalMotor[GMT_Yaw].sensorFeedBack.offset = 0x17E7;
    m_GimbalMotor[GMT_Yaw].pidSpeed.kp = G6020_YAW_SPD_KP;
    m_GimbalMotor[GMT_Yaw].pidSpeed.ki = G6020_YAW_SPD_KI;
    m_GimbalMotor[GMT_Yaw].pidSpeed.kd = G6020_YAW_SPD_KD;
    m_GimbalMotor[GMT_Yaw].pidSpeed.maxOut = G6020_YAW_SPD_MAXOUT;
    m_GimbalMotor[GMT_Yaw].pidSpeed.maxIOut = G6020_YAW_SPD_IMAXOUT;

    m_GimbalMotor[GMT_Yaw].pidPosition.kp = G6020_YAW_POS_KP;
    m_GimbalMotor[GMT_Yaw].pidPosition.ki = G6020_YAW_POS_KI;
    m_GimbalMotor[GMT_Yaw].pidPosition.kd = G6020_YAW_POS_KD;
    m_GimbalMotor[GMT_Yaw].pidPosition.maxOut = G6020_YAW_POS_MAXOUT;
    m_GimbalMotor[GMT_Yaw].pidPosition.maxIOut = G6020_YAW_POS_IMAXOUT;

    m_GimbalMotor[GMT_Pitch].RegistMotor(CAN1, 0x205);
    m_GimbalMotor[GMT_Pitch].sensorFeedBack.offset = 0x1454;//0x1454;
    m_GimbalMotor[GMT_Pitch].controlMode = Motor::POS_MODE;

    m_GimbalMotor[GMT_Pitch].pidSpeed.kp = G6020_PITCH_SPD_KP;
    m_GimbalMotor[GMT_Pitch].pidSpeed.ki = G6020_PITCH_SPD_KI;
    m_GimbalMotor[GMT_Pitch].pidSpeed.kd = G6020_PITCH_SPD_KD;
    m_GimbalMotor[GMT_Pitch].pidSpeed.maxOut = G6020_PITCH_SPD_MAXOUT;
    m_GimbalMotor[GMT_Pitch].pidSpeed.maxIOut = G6020_PITCH_SPD_IMAXOUT;

    m_GimbalMotor[GMT_Pitch].pidPosition.kp = G6020_PITCH_POS_KP;
    m_GimbalMotor[GMT_Pitch].pidPosition.ki = G6020_PITCH_POS_KI;
    m_GimbalMotor[GMT_Pitch].pidPosition.kd = G6020_PITCH_POS_KD;
    m_GimbalMotor[GMT_Pitch].pidPosition.maxOut = G6020_PITCH_POS_MAXOUT;
    m_GimbalMotor[GMT_Pitch].pidPosition.maxIOut = G6020_PITCH_POS_IMAXOUT;

    gimbalFsm.Init();
	p_HostCom = HostCom::Instance();
}

void GimbalController::Update()
{
    gimbalFsm.HandleInput();
    gimbalFsm.Update();

    if(Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_Q, Dr16::KEY_FALL) && Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_CTRL, Dr16::KEY_DOWN))
    {
        BoardPacketManager::Instance()->GetTestPacket().m_testData = 0;
        BoardPacketManager::Instance()->GetTestPacket().SendPacket();
    }

    if(Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_E, Dr16::KEY_FALL) && Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_CTRL, Dr16::KEY_DOWN))
    {
        BoardPacketManager::Instance()->GetTestPacket().m_testData = 1;
        BoardPacketManager::Instance()->GetTestPacket().SendPacket();
    }

    if(Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_R, Dr16::KEY_FALL) && Dr16::Instance()->QueryPcKeyState(Dr16::PC_KEY_CTRL, Dr16::KEY_DOWN))
    {
        BoardPacketManager::Instance()->GetTestPacket().m_testData = 2;
        BoardPacketManager::Instance()->GetTestPacket().SendPacket();
     }

	p_HostCom->hostGimbalFdbAnglePacket.m_yaw = AHRSEstimator::Instance()->GetYaw();
	p_HostCom->hostGimbalFdbAnglePacket.m_pitch = - AHRSEstimator::Instance()->GetRoll();
}

float GimbalController::GetYawRelativeAngleRad()
{
    return m_GimbalMotor[GMT_Yaw].sensorFeedBack.positionFdb;
}

void GimbalFsm::HandleInput()
{
    if(!hasInited)
    {
        if(Time::GetTick() > ticksBeforeInit)
        {
            ChangeState(GimbalStateMouseControl::Instance());
            hasInited = true;
        }
    }
	
   if(Dr16::Instance()->QuerySwState(Dr16::RC_SW_L, Dr16::RC_SW_DOWN))
   {
	   ChangeState(GimbalStateRc::Instance());
       return;
   }

   if(Dr16::Instance()->QuerySwState(Dr16::RC_SW_L, Dr16::RC_SW_MID))
   {
       ChangeState(GimbalStateMouseControl::Instance());
       return;
   }
}

void GimbalFsm::Init()
{
    GimbalStateRc::Instance()->Init(m_pOwner);
    GimbalStateMouseControl::Instance()->Init(m_pOwner);
    SetCurrentState(GimbalStateRc::Instance());

    ticksBeforeInit = 250;
    hasInited = false;
}

void GimbalController::GimbalUseImu(bool _state)
{
    m_UseImu = _state;
    m_GimbalMotor[GMT_Yaw].UseImuW = _state;
    m_GimbalMotor[GMT_Pitch].UseImuW = _state;
}
