#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "RobotEngine.hpp"
#include "MotorFeedback.hpp"

class Motor : public ActuatorEntity
{
public:
    Motor(EntityClassType _classType):
          ActuatorEntity(_classType),
          controlMode{RELAX_MODE},
          pFeedback{nullptr}
    {}

    enum MotorControlModeType
    {
        RELAX_MODE,
        SPD_MODE,
        POS_MODE,
        POS_FOR_NO_SPD_MODE,
        IMU_MODE
    };

    MotorControlModeType controlMode;
    MotorFeedback* pFeedback;
    float speedSet;
    float positionSet;
    float accelerationSet;
    float imuPositionFdb;   

    virtual void Update() = 0;
};

#endif
