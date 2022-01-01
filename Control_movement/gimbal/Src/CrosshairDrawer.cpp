#include "CrosshairDrawer.hpp"
#include "Dr16.hpp"
#include "math.h"

void CrosshairDrawner::Init()
{
    SetDefaultTicksToUpdate(50);

    m_pGimbalController = (GimbalController*)GetOwner()->GetEntity(ECT_GimbalController);
}

void CrosshairDrawner::Update()
{   
    if(Dr16::Instance()->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_UP))
    {
        m_DrawBuffer.operate_tpye = JudgeSystem::GOT_Add;
        m_DrawBuffer.graphic_name[2] = 1;
        m_DrawBuffer.color = JudgeSystem::GCT_Cyan;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Green;
        m_DrawBuffer.graphic_name[2] = 2;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.graphic_name[2] = 3;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.graphic_name[2] = 4;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.graphic_name[2] = 5;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.graphic_name[2] = 6;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.graphic_name[2] = 7;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        /*m_DrawBuffer.color = JudgeSystem::GCT_Purple;
        m_DrawBuffer.graphic_name[2] = 3;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Yellow;
        m_DrawBuffer.graphic_name[2] = 4;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Pink;
        m_DrawBuffer.graphic_name[2] = 5;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Purple;
        m_DrawBuffer.graphic_name[2] = 6;
        m_DrawBuffer.radius = 50;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Team;
        m_DrawBuffer.graphic_name[2] = 7;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Orange;
        m_DrawBuffer.graphic_name[2] = 8;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Yellow;
        m_DrawBuffer.graphic_name[2] = 9;
        m_DrawBuffer.radius = 60;
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);*/

    }
    else if(Dr16::Instance()->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_MID))
    {
        m_DrawBuffer.operate_tpye = JudgeSystem::GOT_Modify;
        float _v = JudgeSystem::Instance()->GameRobotStatus.shooter_heat0_speed_limit;
        float _v2 = _v * _v;
        float _gimbalPitchAngle = - m_pGimbalController->GetGimbalMotor(GimbalController::GMT_Pitch)->sensorFeedBack.positionFdb;
        float _cos = cos(_gimbalPitchAngle);
        float _sin = sin(_gimbalPitchAngle);

        float _time = 0.0f;
        float _atan = 0.0f;
        float _halfArmorLanding = 0.0;
        float _l = 0.0f;
        int16_t _dropPixel;
        float _invV = 1 / _v;

        m_DrawBuffer.graphic_name[2] = 1;
        m_DrawBuffer.color = JudgeSystem::GCT_Orange;

        _time = (_v * _sin + sqrt(_v2 * _sin * _sin + 2.0f * GRAVITY * GIMBAL_HEIGHT)) * INV_GRAVITY; 

        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }

        m_DrawBuffer.start_x = 960;
        m_DrawBuffer.start_y = 540;
        m_DrawBuffer.end_x = 960;
        m_DrawBuffer.end_y = 540 - _dropPixel;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 2;
        

        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;

        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 3;
        m_DrawBuffer.color = JudgeSystem::GCT_Green;
        _time = 0.1f;
        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }

        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;

        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 4;

        _time = 0.2f;
        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }
        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;

        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 5;

        _time = 0.4f;
        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }
        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;
        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 6;

        _time = 0.6f;
        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }
        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;
        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        m_DrawBuffer.graphic_name[2] = 7;

        _time = 0.8f;
        _atan = atan2(_sin - 0.5f * GRAVITY * _time * _invV, _cos);
        _dropPixel = (int16_t)(PIXEL_RATIO * tan(_gimbalPitchAngle - _atan));
        if(_dropPixel > 539)
        {
            _dropPixel = 539;
        }
        m_DrawBuffer.start_y = 540 - _dropPixel;
        m_DrawBuffer.end_y = 540 - _dropPixel;
        _l = _v * _time * _cos / cos(_atan);

        _halfArmorLanding = PIXEL_RATIO * REF_LEN / _l;

        m_DrawBuffer.start_x = 960 - _halfArmorLanding;
        m_DrawBuffer.end_x = 960 + _halfArmorLanding;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);

        /*m_DrawBuffer.color = JudgeSystem::GCT_Cyan;
        m_DrawBuffer.graphic_name[2] = 2;
        m_DrawBuffer.radius = 35 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Purple;
        m_DrawBuffer.graphic_name[2] = 3;
        m_DrawBuffer.radius = 55 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Yellow;
        m_DrawBuffer.graphic_name[2] = 4;
        m_DrawBuffer.radius = 75 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Pink;
        m_DrawBuffer.graphic_name[2] = 5;
        m_DrawBuffer.radius = 95 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Purple;
        m_DrawBuffer.graphic_name[2] = 6;
        m_DrawBuffer.radius = 115 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Team;
        m_DrawBuffer.graphic_name[2] = 7;
        m_DrawBuffer.radius = 135 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Orange;
        m_DrawBuffer.graphic_name[2] = 8;
        m_DrawBuffer.radius = 155 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
        m_DrawBuffer.color = JudgeSystem::GCT_Yellow;
        m_DrawBuffer.graphic_name[2] = 9;
        m_DrawBuffer.radius = 175 + (int16_t)(10.0f * sin(0.002f * (float)GetOwner()->GetTick()));
        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);*/
    }
    else if(Dr16::Instance()->QuerySwState(Dr16::RC_SW_R, Dr16::RC_SW_DOWN))
    {
        /* code */
        m_DrawBuffer.operate_tpye = JudgeSystem::GOT_Delete;

        m_DrawBuffer.graphic_tpye = JudgeSystem::GT_Line;

        m_DrawBuffer.layer = 0;

        m_DrawBuffer.color = JudgeSystem::GCT_Black;

        m_DrawBuffer.width = 2;

        m_DrawBuffer.start_x = 960;

        m_DrawBuffer.start_y = 540;

        m_DrawBuffer.radius = m_Radius;

        JudgeSystem::Instance()->ModfiyShapeOnClient(&m_DrawBuffer);
    }
}
