#include "DjiNineAxisGyro.hpp"

void DjiGyro::Init()
{
    djiGyroEulerAngle.RegistSensor(CAN2,0x401);
		
}

void DjiGyroEulerAngle::Update()
{
    float delta_yaw = 0.0f;
		last_yaw = raw_yaw;
    raw_yaw = ((int16_t)(lastCanMsg.Data[1]<<8 | lastCanMsg.Data[0]))* 0.0001f;
    delta_yaw = last_yaw - raw_yaw;

		if(delta_yaw<-3.14f)
		{
			delta_yaw = 6.28f + delta_yaw;
		}
		else if(delta_yaw>3.14f)
		{
			delta_yaw = 6.28f - delta_yaw;
		}
	
    yaw -=delta_yaw;

    roll = (lastCanMsg.Data[3]<<8 | lastCanMsg.Data[2]) * 0.0001f;
    pitch = (lastCanMsg.Data[5]<<8 | lastCanMsg.Data[4]) * 0.0001f;
    pitch -= 1.53f;
    pitch = -pitch;
}

void DjiGyroAngleSpeed::Update()
{
	
}
