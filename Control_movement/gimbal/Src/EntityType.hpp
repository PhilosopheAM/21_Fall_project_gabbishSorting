#ifndef ENTITYTYPE_HPP
#define ENTITYTYPE_HPP

enum EntityType
{
    ET_SENSOR = 0,
    ET_CONTROLLER,
    ET_ACTUATOR,

    ET_LENGTH
};

enum EntityClassType
{
    ECT_NULL = 0,
    ECT_M3508,
    ECT_M3508Sensor,
    ECT_M2006,
    ECT_M2006Sensor,
    ECT_GM3510,
    ECT_GM3510Sensor,
    ECT_G6020,
    ECT_G6020Sensor,
    ECT_ChassisController,
    ECT_GimbalController,
	ECT_ShooterController,
    ECT_Imu,

    ECT_CrosshairDrawer,

    ECT_LENGTH
};

#endif
