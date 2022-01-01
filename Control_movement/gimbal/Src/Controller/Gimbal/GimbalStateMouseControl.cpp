#include "GimbalStateMouseControl.hpp"
#include "Math.hpp"
#include "Dr16.hpp"
#include "Buzzer.hpp"

GimbalStateMouseControl *pMouseStateDebugHandler;

void GimbalStateMouseControl::Init(GimbalController *pOwner)
{
    pMouseStateDebugHandler = this;

    m_chassis = (ChassisController *)pOwner->GetOwner()->GetEntity(ECT_ChassisController);

    m_pYawMotor = pOwner->GetGimbalMotor(GimbalController::GMT_Yaw);
    m_pPitchMotor = pOwner->GetGimbalMotor(GimbalController::GMT_Pitch);
    m_MinPitch = -0.45f;
    m_MaxPitch = 0.8f;

    YawPid.kp = 45.0f;
    YawPid.ki = 0.0f;
    YawPid.kd = 8.0f;
    YawPid.maxIOut = 0.0f;
    YawPid.maxOut = 50.0f;

    PitchPid.kp = 65.0f;
    PitchPid.ki = 0.0f;
    PitchPid.kd = 4.0f;
    PitchPid.maxIOut = 0.0f;
    PitchPid.maxOut = 60.0f;
    m_PitchSet = 0.0f;

    yawSumErr = 0;
    aimbotOffset = 0.0f;
    offsetSensitivity = 0.03f;

    yaw_filter.SetTau(0.025f);
    yaw_filter.SetUpdatePeriod(pOwner->GetDefaultTicksToUpdate());

    yaw_tag_filter.SetTau(0.05f);
    yaw_tag_filter.SetUpdatePeriod(pOwner->GetDefaultTicksToUpdate());

    pitch_tag_filter.SetTau(0.05f);
    pitch_tag_filter.SetUpdatePeriod(pOwner->GetDefaultTicksToUpdate());

    m_IMU = AHRSEstimator::Instance();
    host_com = HostCom::Instance();
}

void GimbalStateMouseControl::Enter(GimbalController *pOwner)
{
    YawPid.ref = m_IMU->GetAbsYaw();
    PitchPid.ref = -m_IMU->GetRoll();

    m_pYawMotor->controlMode = Motor::SPD_MODE;
    m_pPitchMotor->controlMode = Motor::SPD_MODE;

    m_YawSet = m_IMU->GetAbsYaw();
    m_PitchSet = m_IMU->GetRoll();

    // In case of jump scare.
    yaw_filter.SetResult(m_YawSet);
    yaw_tag_filter.SetResult(m_YawSet);
}

void GimbalStateMouseControl::Execute(GimbalController *pOwner)
{
    //m_YawSet = Math::LoopFloatConstrain(m_YawSet - Dr16::Instance()->GetMouseX() * 0.00005f -Dr16::Instance()->GetRHAxis(), -Math::Pi, Math::Pi);

    float m_yaw_tag = 0.0f;
    float m_pitch_tag = 0.0f;

    m_YawSet -= Dr16::Instance()->GetRHAxis() * 0.15f;
    m_PitchSet += Dr16::Instance()->GetRVAxis() * 0.08f;
    m_YawSet -= Dr16::Instance()->GetMouseX() * 0.0007f;
    m_PitchSet -= Dr16::Instance()->GetMouseY() * 0.00045f;
    m_PitchSet = Math::FloatConstrain(m_PitchSet, m_MinPitch, m_MaxPitch);

    lastPacketTime = host_com->hostGimbalTargetAnglePacket.GetLastTickReceived();

    //yaw_filter.SetInput(m_YawSet);
    //yaw_filter.Update();
    //m_YawSet = yaw_filter.GetResult();

    if (Dr16::Instance()->QueryPcKeyState(Dr16::PC_M_R, Dr16::KEY_DOWN) || Dr16::Instance()->QuerySwState(Dr16::RC_SW_L, Dr16::RC_SW_UP) || Dr16::Instance()->IsTimeout())
    {
        aimbotOffset += Dr16::Instance()->GetMouseZ() * offsetSensitivity;

        if (!host_com->hostGimbalTargetAnglePacket.HasTimeouted())
        {
            //float currentYawErr = host_com->hostGimbalTargetAnglePacket.m_yaw * -0.0075f;
            //float currentPitchErr = host_com->hostGimbalTargetAnglePacket.m_pitch * 0.0007f;
            float currentYawErr = -host_com->hostGimbalTargetAnglePacket.m_yaw;
            float currentPitchErr = host_com->hostGimbalTargetAnglePacket.m_pitch;

            //float errSampleFreq = 1.0f / (Time::GetTick() - lastPacketTime) * 0.001f;
            yawSumErr += currentYawErr;

            m_yaw_tag = 0.006f * currentYawErr;
            //m_pitch_tag = 0.0003f * currentPitchErr;

            m_LastYawErr = currentYawErr;
            m_LastPitchErr = currentPitchErr;

            m_YawSet += m_yaw_tag;
            m_PitchSet += m_pitch_tag;

            //m_YawSet += currentYawErr;
            //m_PitchSet += currentPitchErr;
        }
        else
        {
            //Buzzer::Instance()->Off();
        }
    }
    else
    {
        aimbotOffset = 0.0f;
        yawSumErr = 0.0f;
    }

    // host_com->hostGimbalTargetAnglePacket.m_seq = 880.0f + Dr16::Instance()->GetRVAxis() * 440.0f;
    // host_com->hostGimbalTargetAnglePacket.SendPacket();

    yaw_tag_filter.SetInput(m_YawSet);
    yaw_tag_filter.Update();

    pitch_tag_filter.SetInput(m_PitchSet);
    pitch_tag_filter.Update();

    m_YawSet = yaw_tag_filter.GetResult();
    m_PitchSet = pitch_tag_filter.GetResult();

    YawPid.ref = m_YawSet;
    PitchPid.ref = m_PitchSet;

    YawPid.fdb = m_IMU->GetAbsYaw();
    PitchPid.fdb = m_IMU->GetRoll();

    YawPid.UpdateResult();
    PitchPid.UpdateResult();

    m_pYawMotor->speedSet = YawPid.result - m_chassis->GetChassisRotateCompensation() * 0.25f;
    m_pPitchMotor->speedSet = PitchPid.result - aimbotOffset;
}

void GimbalStateMouseControl::Exit(GimbalController *pOwner)
{
}
