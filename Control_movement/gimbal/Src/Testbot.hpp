#ifndef TESTBOT_HPP
#define TESTBOT_HPP

#include "RobotEngine.hpp"
#include "ChassisController.hpp"
#include "GimbalController.hpp"
#include "ShooterController.hpp"

class Testbot : public RobotEngine
{
private:
    ChassisController chassisController;
    GimbalController gimbalController;
    ShooterController shooterController;
public:
    Testbot() : RobotEngine(this)
    {
    }
};

#endif
