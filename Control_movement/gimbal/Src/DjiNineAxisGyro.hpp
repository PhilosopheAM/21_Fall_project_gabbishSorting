#ifndef DJI_NINEAXIS_GYRO_HPP
#define DJI_NINEAXIS_GYRO_HPP

#include "CanMsgDispatcher.hpp"
#include "CanMsgHandler.hpp"

class DjiGyroEulerAngle : public CanMsgHandler
{
public:
    DjiGyroEulerAngle(){;}
    float pitch;
    float yaw;
    float roll;

    float last_yaw;
		float raw_yaw;
			
    CAN_TypeDef* can;
    uint32_t canId;

    void RegistSensor(CAN_TypeDef* _can, int _canId)
    {
        can = _can;
        canId = _canId;
        CanMsgDispatcher::Instance()->RegisterHandler(can, canId, this);
    }
		
    virtual void Update();
};

class DjiGyroAngleSpeed : public CanMsgHandler
{
public:
    DjiGyroAngleSpeed(){;}
    float w_pitch;
    float w_yaw;
    float w_roll;

			
    CAN_TypeDef* can;
    uint32_t canId;

    void RegistSensor(CAN_TypeDef* _can, int _canId)
    {
        can = _can;
        canId = _canId;
        CanMsgDispatcher::Instance()->RegisterHandler(can, canId, this);
    }
		
    virtual void Update();
};

class DjiGyro
{
public:
    static DjiGyro* Instance()
    {
        static DjiGyro Instance;
        return &Instance;
    }
    
    void Init();
		DjiGyroAngleSpeed djiGyroAngleSpeed;
    DjiGyroEulerAngle djiGyroEulerAngle;

private:
    DjiGyro(){;}
};


#endif
