#include "ChassisController.hpp"
#include "ChassisStateRc.hpp"
#include "arm_math.h"
#include "Led.hpp"

const float ChassisController::M3508StandardSpeedKp = 2000.0f;
const float ChassisController::M3508StandardSpeedKi = 0.0f;
const float ChassisController::M3508StandardSpeedKd = 4.5f;
const float ChassisController::M3508StandardSpeedMaxout = 5500.0f;
const float ChassisController::M3508StandardSpeedIMaxout = 0.0f;
const float ChassisController::M3508MaxSpeed = 60.0f;

const float ChassisController::RadiusOfWheel = 0.07625f;// Meter

const float ChassisController::CapCutOffVoltage = 12.0f;
const float ChassisController::CapRestartVoltage = 14.0f;

ChassisController::ChassisController() : ControllerEntity(ECT_ChassisController),
	m_MaxWheelSpd(M3508MaxSpeed),m_set_max_power(53.0f),
	chassisFsm(this)
{}

void ChassisController::Init()
{
    SetDefaultTicksToUpdate(1);

    for(int i = 0; i < CWT_LENGTH; ++i)
    {
        m_ChassisWheel[i].RegistMotor(CAN1, 0x201 + i);
	    m_ChassisWheel[i].controlMode = Motor::SPD_MODE;
        m_ChassisWheel[i].pidSpeed.kp = M3508StandardSpeedKp;
        m_ChassisWheel[i].pidSpeed.ki = M3508StandardSpeedKi;
        m_ChassisWheel[i].pidSpeed.kd = M3508StandardSpeedKd;
        m_ChassisWheel[i].pidSpeed.maxOut = M3508StandardSpeedMaxout;
        m_ChassisWheel[i].pidSpeed.maxIOut = M3508StandardSpeedIMaxout;
    }

    m_pDr16 = Dr16::Instance();

	m_V_start_limit  = 2.5f;
	
	voltage_filter.SetTau(0.1f);
	voltage_filter.SetUpdatePeriod(1);
	
	Vx_filters[0].SetTau(0.15f);
	Vx_filters[0].SetUpdatePeriod(1);
	
	Vx_filters[1].SetTau(0.05f);
	Vx_filters[1].SetUpdatePeriod(1);
	
	Vx_filters[2].SetTau(0.05f);
	Vx_filters[2].SetUpdatePeriod(1);
	
	Vy_filters[0].SetTau(0.02f);
	Vy_filters[0].SetUpdatePeriod(1);
	
	Vy_filters[1].SetTau(0.02f);
	Vy_filters[1].SetUpdatePeriod(1);
	
	Vz_filters[0].SetTau(0.01f);
	Vz_filters[0].SetUpdatePeriod(1);
	
	Vz_filters[1].SetTau(0.02f);
	Vz_filters[1].SetUpdatePeriod(1);
	
    chassisFsm.Init();
}

uint16_t power ;

void ChassisController::Update()
{
    chassisFsm.HandleInput();

    chassisFsm.Update();

    ChassisSpd2MotorSpd();

	power =0;
	
	if(power>=39 &&power <=121)
	{
		m_set_max_power = power-3;
	}
	
	FilterUpdate();
	MaxDeltaVLimit();
	
	MotorSpd2ChassisSpdFdb();
	
    if(!m_ChassisWheel[CWT_RightFront].sensorFeedBack.IsTimeout())
    {
        Led::On(Led::DebugA0);
    }
    else
    {
        Led::Off(Led::DebugA0);
    }

    if(!m_ChassisWheel[CWT_LeftFront].sensorFeedBack.IsTimeout())
    {
        Led::On(Led::DebugB1);
    }
    else
    {
        Led::Off(Led::DebugB1);
    }

    if(!m_ChassisWheel[CWT_LeftRear].sensorFeedBack.IsTimeout())
    {
        Led::On(Led::DebugC2);
    }
    else
    {
        Led::Off(Led::DebugC2);
    }

    if(!m_ChassisWheel[CWT_RightRear].sensorFeedBack.IsTimeout())
    {
        Led::On(Led::DebugD3);
    }
    else
    {
        Led::Off(Led::DebugD3);
    }
}

void ChassisController::MotorSpd2ChassisSpdFdb()
{
    float temp_speed[4] = {0};

    for (int i = 0; i < 4; i++)
    {
        temp_speed[i] = m_ChassisWheel[i].pFeedback->speedFdb * RadiusOfWheel ;// meter / second
    }
    
    m_VxFdb = (- temp_speed[0] + temp_speed[1] + temp_speed[2] - temp_speed[3]) * 0.25f;
    m_VyFdb = (- temp_speed[0] - temp_speed[1] + temp_speed[2] + temp_speed[3]) * 0.25f;
    m_VwFdb = (- temp_speed[0] - temp_speed[1] - temp_speed[2] - temp_speed[3]) * 0.707107f * 0.25f;
	
	m_PosX += m_VxFdb*0.001f;
	m_PosY += m_VyFdb*0.001f;
	
	m_V_fdb_length = m_VxFdb * m_VxFdb + m_VyFdb * m_VyFdb + m_VwFdb * m_VwFdb;
}

void ChassisController::ChassisSpd2MotorSpd()
{
    float temp_vector[4] = {- m_Vx - m_Vy - m_Vw,
                            + m_Vx - m_Vy - m_Vw,
                            + m_Vx + m_Vy - m_Vw,
                            - m_Vx + m_Vy - m_Vw};
    
    float temp_max = 0.0f;
    for(int i = 0;i < 4;++i){
        if(temp_max < fabs(temp_vector[i])){
            temp_max = fabs(temp_vector[i]);
        }
    }

	m_V_set_length = m_Vx*m_Vx+m_Vy*m_Vy+m_Vw*m_Vw;
	
	float limit_coe = 0.6f + 0.4f/(1.0f + powf(1.5f,-0.2f*(m_cap_coe-30.0f)));
	
	//limit_coe = m_buffer_coe * limit_coe;
	
    for(int i = 0;i < 4;++i)
    {
        if(temp_max > m_MaxWheelSpd)
        {
            temp_vector[i] = temp_vector[i] / temp_max * m_MaxWheelSpd;
        }
        m_ChassisWheel[i].speedSet = temp_vector[i]*limit_coe;
    }
}

void ChassisFsm::HandleInput()
{	
	return;
}

void ChassisFsm::Init()
{
    ChassisStateRc::Instance()->Init(m_pOwner);
    SetCurrentState(ChassisStateRc::Instance());
}

void ChassisController::FilterUpdate()
{
	Vx_filters[0].SetInput(m_Vx);
	Vx_filters[0].Update();
	Vx_filters[1].SetInput(Vx_filters[0].GetResult());
	Vx_filters[1].Update();
	Vx_filters[2].SetInput(Vx_filters[1].GetResult());
	Vx_filters[2].Update();
	m_Vx = Vx_filters[2].GetResult();
	
	Vy_filters[0].SetInput(m_Vy);
	Vy_filters[0].Update();
	Vy_filters[1].SetInput(Vy_filters[0].GetResult());
	Vy_filters[1].Update();
	m_Vy = Vy_filters[1].GetResult();
	
	Vz_filters[0].SetInput(m_Vw);
	Vz_filters[0].Update();
	Vz_filters[1].SetInput(Vz_filters[0].GetResult());
	Vz_filters[1].Update();
	m_Vw = Vz_filters[1].GetResult();
}

void ChassisController::MaxDeltaVLimit()
{
	float VxFdbAbs = m_VxFdb* 13.1148f;
	float delta = m_Vx - VxFdbAbs;
	
	if(VxFdbAbs <0.0f)
	{
		VxFdbAbs = -VxFdbAbs;
	}
	
	float max_delta = 1.2f + (m_VxFdb*m_VxFdb) * 13.1148f/50.0f;
	
	if(max_delta > 3.0f)
	{
		max_delta = 3.0f;
	}
	
	if(m_Vx * m_VxFdb < -0.01f)
	{
		max_delta*=0.005f;
	}
	
	if(delta>max_delta)
	{
		m_Vx = VxFdbAbs + max_delta;
	}
	else if(delta<-max_delta)
	{
		m_Vx = -VxFdbAbs -max_delta;
	}
	
	for(int i = 0; i < CWT_LENGTH; ++i)
    {
        m_ChassisWheel[i].pidSpeed.maxOut = M3508StandardSpeedMaxout + VxFdbAbs * 200.0f;
    }
}
