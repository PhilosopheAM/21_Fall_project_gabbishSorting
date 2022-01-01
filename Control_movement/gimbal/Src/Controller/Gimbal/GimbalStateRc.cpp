#include "GimbalStateRc.hpp"
#include "Math.hpp"

#include "Crc.hpp"
#include "Led.hpp"

void GimbalStateRc::Init(GimbalController* pOwner)
{
    m_pYawMotor = pOwner->GetGimbalMotor(GimbalController::GMT_Yaw);
    m_pPitchMotor = pOwner->GetGimbalMotor(GimbalController::GMT_Pitch);
    m_MinPitch = - 0.49f;
    m_MaxPitch = 0.58f;
    
    m_YawSet = m_pYawMotor->pFeedback->positionFdb;
    m_PitchSet = m_pPitchMotor->pFeedback->positionFdb;

}

void GimbalStateRc::Enter(GimbalController* pOwner)
{
    m_pYawMotor->controlMode = Motor::POS_MODE;
    m_pPitchMotor->controlMode = Motor::POS_MODE;

    m_YawSet = m_pYawMotor->sensorFeedBack.positionFdb;
    m_PitchSet = m_pPitchMotor->sensorFeedBack.positionFdb;

    //pOwner->SetImuYawZero();
}

void GimbalStateRc::Execute(GimbalController* pOwner)
{
    m_YawSet -= Dr16::Instance()->GetRHAxis() * 0.01f;
    m_PitchSet -= Dr16::Instance()->GetRVAxis() * 0.006f;
    m_YawSet -= Dr16::Instance()->GetMouseX() * 0.0001f;
    m_PitchSet += Dr16::Instance()->GetMouseY() * 0.00015f;
    m_YawSet = Math::LoopFloatConstrain(m_YawSet, -PI, PI);
    m_PitchSet = Math::FloatConstrain(m_PitchSet, m_MinPitch, m_MaxPitch);

    m_pYawMotor->positionSet = m_YawSet;
    m_pPitchMotor->positionSet = m_PitchSet;
}

void GimbalStateRc::Exit(GimbalController* pOwner)
{

}
