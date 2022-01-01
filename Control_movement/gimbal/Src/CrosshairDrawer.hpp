#ifndef CROSSHAIRDRAWER_HPP
#define CROSSHAIRDRAWER_HPP

#include "RobotEngine.hpp"
#include "JudgeSystem.hpp"
#include "GimbalController.hpp"

#define GRAVITY 9.7925f
#define INV_GRAVITY 0.1021189686f
#define GIMBAL_HEIGHT 0.45f
#define ARMOR_LENGTH 0.13f
#define PIXEL_RATIO 576.0f
#define REF_LEN 0.15f

class CrosshairDrawner : public ControllerEntity
{
private:
    JudgeSystem::JS_GraphicData m_DrawBuffer;
    GimbalController* m_pGimbalController;
    uint16_t m_Radius;
public:
    CrosshairDrawner() : ControllerEntity(ECT_CrosshairDrawer),
                         m_Radius(50)
    {}

    virtual void Init();
    virtual void Update();
};

#endif
