#ifndef CHASSISCONTROLLER_HPP
#define CHASSISCONTROLLER_HPP

#include <math.h>
#include "RobotEngine.hpp"
#include "StateMachine.hpp"
#include "Dr16.hpp"
#include "M3508.hpp"
#include "ChassisCtrlMsg.hpp"
#include "math_first_order_filter.h"

// Forward declaration
class ChassisController;

class ChassisFsm : public StateMachine<ChassisController>
{
public:
    ChassisFsm(ChassisController* _pOwner) : StateMachine<ChassisController>(_pOwner){}
    void HandleInput();

    virtual void Init();
};

class ChassisController : public ControllerEntity
{
private:
    M3508 m_ChassisWheel[4];

    float m_Vx;
    float m_Vy;
    float m_Vw;
    float m_V_set_length;

    float m_V_start_limit;

    float m_VxFdb;
    float m_VyFdb;
    float m_VwFdb;
	float m_V_fdb_length;

    uint8_t relay_state;//0: relay off  1:relay on
	uint8_t chassis_state;

	float m_cap_voltage;
	float m_cap_coe;

	float m_buffer_coe;

    float m_MaxWheelSpd;
	float m_set_max_power;

	FirstOrderFilter voltage_filter;
	FirstOrderFilter Vx_filters[3];
	FirstOrderFilter Vy_filters[2];
	FirstOrderFilter Vz_filters[2];

    Dr16* m_pDr16;
public:
    enum ChassisWheelType
    {
        CWT_RightFront = 0,
        CWT_LeftFront,
        CWT_LeftRear,
        CWT_RightRear,

        CWT_LENGTH
    };

    ChassisFsm chassisFsm;

    const static float M3508StandardSpeedKp;
    const static float M3508StandardSpeedKi;
    const static float M3508StandardSpeedKd;
    const static float M3508StandardSpeedMaxout;
    const static float M3508StandardSpeedIMaxout;
    const static float M3508MaxSpeed;

	const static float CapCutOffVoltage;
    const static float CapRestartVoltage;
    const static float RadiusOfWheel;
	
	float m_PosX;
	float m_PosY;
	
    ChassisController();

    virtual void Init();
    virtual void Update();

    void ChassisSpd2MotorSpd();
	void MotorSpd2ChassisSpdFdb();
	void MaxPowerSet();
	void CapacitorControl();
	void MaxDeltaVLimit();
	void RelayControl();
	void FilterUpdate();

    void Stop()
    {
        m_Vx = 0.0f;
        m_Vy = 0.0f;
        m_Vw = 0.0f;
    }

    void SetVx(float vx){m_Vx = vx;}
    void SetVy(float vy){m_Vy = vy;}
    void SetVw(float vw){m_Vw = vw;}

    float GetVx(){ return m_Vx; }
    float GetVy(){ return m_Vy; }
    float GetVw(){ return m_Vw; }

    float GetVxFdb(){ return m_VxFdb; }
    float GetVyFdb(){ return m_VyFdb; }
    float GetVwFdb(){ return m_VwFdb; }

    M3508* GetChassisMotor(ChassisWheelType _wheel){ return &m_ChassisWheel[_wheel]; }
};

#endif
